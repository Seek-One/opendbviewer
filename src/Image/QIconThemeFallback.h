/*
 * QIconFallback.h
 *
 *  Created on: 24 sept. 2012
 *      Author: ebeuque
 */

#ifndef QICONFALLBACK_H_
#define QICONFALLBACK_H_

#include <QIcon>
#include <QDir>

/*
 *
 */
class QIconThemeFallback {
public:
	QIconThemeFallback();
	virtual ~QIconThemeFallback();

	/**
	 * Define the fallback theme name
	 */
	static void setThemeName(const QString& szThemeName);

	/**
	 * Return the fallback theme name
	 */
	static QString themeName();

	/**
	 * Returns the QIcon corresponding to name in the current icon theme.
	 * If no such icon is found in the current theme, it will try in the fallback theme.
	 * If no such icon is found in the fallback theme, fallback is returned instead.
	 *
	 * @see QIcon::fromTheme for more information
	 */
	static QIcon fromThemeThenFallback(const QString& szName, const QIcon & fallback = QIcon());

	/**
	 * Returns the QIcon corresponding to name in the current fallback icon theme.
	 * If no such icon is found in the current fallback theme, fallback is returned instead.
	 *
	 * @see QIcon::fromTheme for more information
	 */
	static QIcon fromThemeFallback(const QString& szName, const QIcon & fallback = QIcon());

private:
	QString m_szThemeName;

	QDir m_contentDir;

	void updateThemeInfo();
};

#endif /* QICONTHEMEFALLBACK_H_ */
