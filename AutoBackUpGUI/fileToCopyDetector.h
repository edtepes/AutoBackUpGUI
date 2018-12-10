#pragma once
#include <QFileSystemWatcher>
#include <QObject>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QMultiMap>
#include <QStringList>

class fileToCopyDetector : public QFileSystemWatcher
{
	Q_OBJECT

public:
	fileToCopyDetector(QObject *parent); //base constructor initialized without any directories
	fileToCopyDetector(const QStringList &paths, QObject *parent); //constructor that accepts a list of directories to watch - used for testing
	~fileToCopyDetector();

	bool addPathToWatch(const QString &path); //add a path to directory to watch

private:
	QMultiMap<QString, QString> dirsAndFilesMap; //contains an index of files already contained in destination directories

	void removeFiles(QStringList &files, const QString &path); //handles the case when a file is removed from a directory being watched

public slots:
	void slotWatchNewDir(const QString &dirPath); //receives signal that a new directory needs to be watched
	void slotWatchNewDir(const QStringList &paths); //receives signal of multiple directories to be watched, overloads preceding function

private slots:
	void slotDirChanged(const QString &path); //internal slot that recieves signal that a watched directory was modified

signals:
	void newFileToTransfer(const QString &filePath); //emits path to newly inserted file in directory
	void dirRemoved(const QString &path); //emits path of directory that was removed
};