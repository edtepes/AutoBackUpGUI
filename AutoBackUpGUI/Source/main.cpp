#include "AutoBackUpGUI.h"
#include <QtWidgets/QApplication>
#include <QString>
#include "fileToCopyDetector.h"
#include "fileCopyHandler.h"
#include "dataBase.h"
#include "googleDriveInterface.h"
#include "googleFileUploadQueue.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);


	//establish all connections here for different components of program - names to be changed
	
	fileToCopyDetector* detectTest = new fileToCopyDetector(&a);
	dataBase* dbTest = new dataBase(&a);
	fileCopyHandler* copyTest = new fileCopyHandler(&a);
	googleDriveInterface* googleDrive = new googleDriveInterface(&a);
	googleFileUploadQueue* googleQueue = new googleFileUploadQueue(&a);

	QObject::connect(detectTest, SIGNAL(newFileToTransfer(const QString)), copyTest, SLOT(slotFileToCopy(const QString)));
	QObject::connect(copyTest, SIGNAL(newDirToWatch(const QString)), detectTest, SLOT(slotWatchNewDir(const QString)));
	QObject::connect(copyTest, SIGNAL(newDirToWatch(const QString)), dbTest, SLOT(slotAddNewDirToDB(const QString)));
	QObject::connect(dbTest, SIGNAL(dirsLoadedFromDB(const QStringList)), detectTest, SLOT(slotWatchNewDir(const QStringList)));
	QObject::connect(detectTest, SIGNAL(dirRemoved(const QString)), dbTest, SLOT(slotRemoveFromDB(const QString)));
	QObject::connect(dbTest, SIGNAL(destLoadedFromDB(const QString)), copyTest, SLOT(slotNewDestDir(const QString)));
	QObject::connect(dbTest, SIGNAL(destLoadedFromDB(const QString)), detectTest, SLOT(slotDestinationSet(const QString)));
	QObject::connect(googleDrive, SIGNAL(newDriveApiRefreshToken(const QString)), dbTest, SLOT(slotAddRefreshTokenToDB(const QString)));
	QObject::connect(dbTest, SIGNAL(refreshTokenLoadedFromDB(const QString)), googleDrive, SLOT(slotSetRefreshToken(const QString)));
	QObject::connect(detectTest, SIGNAL(newFileToTransfer(const QString)), googleQueue, SLOT(slotAddFileToQueue(const QString)));
	QObject::connect(googleQueue, SIGNAL(nextFileToUpload(const QString)), googleDrive, SLOT(slotUploadFileToDrive(const QString)));
	QObject::connect(googleDrive, SIGNAL(readyForNextUpload()), googleQueue, SLOT(slotShouldSendNextFile()));

	AutoBackUpGUI w;
	w.setupExternalConnections(dbTest, detectTest, copyTest);
	w.show();

	dbTest->emitData();

	return a.exec();
}