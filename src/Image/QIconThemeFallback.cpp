/*
 * QIconThemeFallback.cpp
 *
 *  Created on: 24 sept. 2012
 *      Author: ebeuque
 */

#include "QIconThemeFallback.h"

#include <QApplication>
#include <QCache>

#include <QIcon>
#include <QString>
#include <QDir>

#include <QSettings>
#include <QPainter>
#include <QPixmap>
#include <QPixmapCache>
#include <QStringBuilder>

#include <QStyle>
#include <QStyleOption>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define USE_QICONENGINEV2_MERGED QT_VERSION
#define USE_QICONENGINE_LESS_HOOKS
#endif
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#define USE_QTENGINEDEFAULT
#endif

#ifdef USE_QICONENGINEV2_MERGED

#include <QIconEngine>
#define QIconEngineDefault QIconEngine

#else

#include <QLatin1Literal>
#include <QIconEngineV2>
#ifdef USE_QTENGINEDEFAULT
#define QIconEngineDefault QIconEngine
#else
#define QIconEngineDefault QIconEngineV2
#endif // USE_QTENGINEDEFAULT

#endif // USE_QICONENGINEV2_MERGED


static void q_cleanup_fallback_icon_cache();


typedef QCache<QString, QIcon> FallbackIconCache;

FallbackIconCache *qFallbackIconCache()
{
    static FallbackIconCache *pointer;
    if (!pointer) {
        pointer = new FallbackIconCache;
        qAddPostRoutine(q_cleanup_fallback_icon_cache);
    }
    return pointer;
}
Q_GLOBAL_STATIC(QIconThemeFallback, qIconThemeFallback);

class QIconEntry
{
private:
	QString m_filePath;
	int m_size;

	QPixmap* m_pixmap;

public:
	QIconEntry(const QString& filePath, const int& size) : m_filePath(filePath), m_size(size)
	{
		m_pixmap = NULL;
	}

	virtual ~QIconEntry()
	{
		if(m_pixmap){
			delete m_pixmap;
			m_pixmap = NULL;
		}
	}

	int size() const
	{
		return m_size;
	}

	const QString& filePath() const
	{
		return m_filePath;
	}

	const QPixmap* pixmap()
	{
		if(!m_pixmap){
			m_pixmap = new QPixmap(m_filePath);
		}
		return m_pixmap;
	}
};

// Import from gui/styles/qstylehelper_p.h

template <typename T>
        struct HexString
{
    inline HexString(const T t)
        : val(t)
    {}

    inline void write(QChar *&dest) const
    {
        const ushort hexChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

        const char *c = reinterpret_cast<const char *>(&val);
        for (uint i = 0; i < sizeof(T); ++i) {
            *dest++ = hexChars[*c & 0xf];
            *dest++ = hexChars[(*c & 0xf0) >> 4];
            ++c;
        }
    }

    const T val;
};

// specialization to enable fast concatenating of our string tokens to a string

template <typename T>
        struct QConcatenable<HexString<T> >
{
    typedef HexString<T> type;

    enum { ExactSize = true };

    static int size(const HexString<T> &) { return sizeof(T) * 2; }

    static inline void appendTo(const HexString<T> &str, QChar *&out) { str.write(out); }

    typedef QString ConvertTo;
};

typedef QList<QIconEntry*> QIconEntryList;

class QIconFallbackLoader : public QObject
{
private:
	QHash<QString, QIconEntryList> m_listIconEntry;

public:
	QIconFallbackLoader(){}

	virtual ~QIconFallbackLoader()
	{
		QIconEntryList list;
		QIconEntry* pEntry;

		QHash<QString, QIconEntryList>::iterator iter;
		iter = m_listIconEntry.begin();
		while(iter != m_listIconEntry.end())
		{
			QIconEntryList listEntry = (iter).value();

			while(!listEntry.isEmpty()) {
				pEntry = listEntry.takeFirst();
				if(pEntry){
					delete pEntry;
					pEntry = NULL;
				}
			}

			iter = m_listIconEntry.erase(iter);
		}
	}

	void addEntry(const QString& szName, const QString& filePath, const int& size)
	{
		QIconEntry* pEntry;
		pEntry = new QIconEntry(filePath, size);
		if(m_listIconEntry.contains(szName)){
			QIconEntryList entryList = m_listIconEntry.value(szName);
			entryList.append(pEntry);
			m_listIconEntry.insert(szName, entryList);
		}else{
			QIconEntryList entryList;
			entryList.append(pEntry);
			m_listIconEntry.insert(szName, entryList);
		}
	}

	QIconEntryList entryList(const QString& szName)
	{
		if(m_listIconEntry.contains(szName)){
			return m_listIconEntry.value(szName);
		}
		return QIconEntryList();
	}

};

Q_GLOBAL_STATIC(QIconFallbackLoader, qIconFallbackLoaderInstance);

class QIconFallbackLoaderEngine : public QIconEngineDefault
{
private:
	QString m_iconName;

public:
	QIconFallbackLoaderEngine(const QString& iconName = QString()) : m_iconName(iconName)
	{

	};

	~QIconFallbackLoaderEngine()
	{

	};

#ifdef USE_QTENGINEDEFAULT
    QIconEngine *clone() const
    {
    	return new QIconFallbackLoaderEngine(m_iconName);
    }
#endif

	void virtual_hook(int id, void *data)
	{
	    switch (id) {
#ifndef USE_QICONENGINE_LESS_HOOKS
	    case QIconEngineDefault::AvailableSizesHook:
	    	{
	    		QIconEngineDefault::AvailableSizesArgument &arg = *reinterpret_cast<QIconEngineDefault::AvailableSizesArgument*>(data);

	            QIconEntryList entryList = qIconFallbackLoaderInstance()->entryList(m_iconName);
	            arg.sizes.clear();

	            // Gets all sizes from the DirectoryInfo entries
	            for (int i = 0 ; i < entryList.size() ; ++i) {
	                int size = entryList.at(i)->size();
	                arg.sizes.append(QSize(size, size));
	            }
	        }

	        break;

	    case QIconEngineDefault::IconNameHook:
	        {
	            QString &name = *reinterpret_cast<QString*>(data);
	            name = m_iconName;
	        }
	        break;
#endif
	    default:
	    	QIconEngineDefault::virtual_hook(id, data);
	        break;
	    }
	};

	QIconEntry* entryForSize(const QSize &size)
	{
		QIconEntry* pBestEntry = NULL;
		QIconEntry* pCurEntry = NULL;

        QIconEntryList entryList = qIconFallbackLoaderInstance()->entryList(m_iconName);

        // Gets all sizes from the DirectoryInfo entries
        for (int i = 0 ; i < entryList.size() ; ++i) {
        	pCurEntry = entryList.at(i);
        	if(!pBestEntry){
        		pBestEntry = pCurEntry;
        	}else if((pCurEntry->size() <= size.width()) && (pCurEntry->size() > pBestEntry->size())){
        		pBestEntry = pCurEntry;
        	}
        }

        return pBestEntry;
	}

	QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
	{
		//ensureLoaded();
		QIconEntry *entry = entryForSize(size);
		if (entry){
			const QPixmap* pBasePixmap = entry->pixmap();
			if(pBasePixmap){

			    int actualSize = qMin(size.width(), size.height());

			    QString key = QLatin1String("$qt_fallbacktheme_")
			                  % HexString<qint64>(pBasePixmap->cacheKey())
			                  % HexString<int>(mode)
			                  % HexString<qint64>(qApp->palette().cacheKey())
			                  % HexString<int>(actualSize);

			    QPixmap cachedPixmap;

			    if (QPixmapCache::find(key, &cachedPixmap)) {
			        return cachedPixmap;
			    } else {
			        QStyleOption opt(0);
			        opt.palette = qApp->palette();
			        cachedPixmap = qApp->style()->generatedIconPixmap(mode, *pBasePixmap, &opt);
			        QPixmapCache::insert(key, cachedPixmap);
			    }

			    return cachedPixmap;
			}
		}

		return QPixmap();
	}

	void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)
	{
	    QSize pixmapSize = rect.size();

	#if defined(Q_WS_MAC)
	    pixmapSize *= qt_mac_get_scalefactor();
	#endif

	    painter->drawPixmap(rect, pixmap(pixmapSize, mode, state));
	};

	QList<QSize> availableSizes(QIcon::Mode mode, QIcon::State state)
	{
		QList<QSize> listSizes;

		QIconEntryList entryList = qIconFallbackLoaderInstance()->entryList(m_iconName);
		// Gets all sizes from the DirectoryInfo entries
		for (auto pEntry : entryList)
		{
			listSizes.append(QSize(pEntry->size(), pEntry->size()));
		}

		return listSizes;
	}

};

static void q_cleanup_fallback_icon_cache()
{
	FallbackIconCache* pCache = qFallbackIconCache();
	if(pCache){
		pCache->clear();
		delete pCache;
	}
}

QIconThemeFallback::QIconThemeFallback()
{

}

QIconThemeFallback::~QIconThemeFallback()
{

}

void QIconThemeFallback::setThemeName(const QString& szThemeName)
{
	qIconThemeFallback()->m_szThemeName = szThemeName;
	qIconThemeFallback()->updateThemeInfo();
}

QString QIconThemeFallback::themeName()
{
	return qIconThemeFallback()->m_szThemeName;
}

QIcon QIconThemeFallback::fromThemeThenFallback(const QString& szName, const QIcon & fallback)
{
	return QIcon::fromTheme(szName, fromThemeFallback(szName, fallback));
}

QIcon QIconThemeFallback::fromThemeFallback(const QString& szName, const QIcon & fallback)
{
	QIcon icon;
	if (qFallbackIconCache()->contains(szName)) {
		icon = *qFallbackIconCache()->object(szName);
	} else {
		auto pLoaderEngine = new QIconFallbackLoaderEngine(szName);
		auto pCachedIcon = new QIcon(pLoaderEngine);
		qFallbackIconCache()->insert(szName, pCachedIcon);
		icon = *pCachedIcon;
	}

	// Note the qapp check is to allow lazy loading of static icons
	// Supporting fallbacks will not work for this case.
	if (qApp && icon.availableSizes().isEmpty()){
		return fallback;
	}

	return icon;
}

void QIconThemeFallback::updateThemeInfo()
{
	// Delete all cached icon
	qFallbackIconCache()->clear();

	QFile themeIndex;
    QStringList iconDirs = QIcon::themeSearchPaths();
    for ( int i = 0 ; i < iconDirs.size() ; ++i) {

        QDir iconDir(iconDirs[i]);

        QString szThemeDir = iconDir.path() + QLatin1Char('/') + m_szThemeName;
        themeIndex.setFileName(szThemeDir + QLatin1String("/index.theme"));
        if (themeIndex.exists()) {
            m_contentDir.setPath(szThemeDir);
            break;
        }
    }

#ifndef QT_NO_SETTINGS
    QStringList listDirectory;

    QStringList listFilters;
    listFilters.append("*.png");
    listFilters.append("*.svg");

    if (themeIndex.exists()) {
        const QSettings indexReader(themeIndex.fileName(), QSettings::IniFormat);

        // Load all directory that contains theme icon
        QStringListIterator keyIterator(indexReader.allKeys());
        while (keyIterator.hasNext()) {
            const QString key = keyIterator.next();
            if (key == QLatin1String("Icon Theme/Directories")) {
            	listDirectory = indexReader.value(key).toStringList();
            	break;
            }
        }

        QStringListIterator directoryIterator(listDirectory);
        while (directoryIterator.hasNext()) {
            const QString dirKey = directoryIterator.next();

            QDir curDir = QDir(m_contentDir.path() + "/" + dirKey);
            if(curDir.exists()){
				QStringList listFiles = curDir.entryList(listFilters, QDir::Files, QDir::Name);

				int iIconSize = indexReader.value(dirKey+"/Size").toInt();

				QStringListIterator filesIterator(listFiles);
				while(filesIterator.hasNext()){
					const QString szFile = filesIterator.next();

					QFileInfo iconFile(curDir.filePath(szFile));
					QString szIconName = iconFile.baseName();

					qIconFallbackLoaderInstance()->addEntry(szIconName, iconFile.filePath(), iIconSize);
				}
            }else{
            	qWarning("Icon theme path invalid: %s", qPrintable(dirKey));
            }
        }

    }

#endif //QT_NO_SETTINGS
}
