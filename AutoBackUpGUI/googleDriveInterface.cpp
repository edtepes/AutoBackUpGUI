#include "googleDriveInterface.h"

googleDriveInterface::googleDriveInterface(QObject *parent) : QObject(parent){
}

googleDriveInterface::~googleDriveInterface() {}

void
googleDriveInterface::getAuthentication() {
	auto google = new QOAuth2AuthorizationCodeFlow;
	google->setScope("https://www.googleapis.com/auth/drive");
	QObject::connect(google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);

	QJsonDocument credentials = getCredentials("googleDriveCredentials.json"); //json file data is loaded into credentials
	
	//parse JSON
	const auto object = credentials.object();
	const auto settingsObject = object["installed"].toObject();
	const QUrl authUri(settingsObject["auth_uri"].toString());
	const auto clientID = settingsObject["client_id"].toString();
	const QUrl tokenUri(settingsObject["token_uri"].toString());
	const auto clientSecret(settingsObject["client_secret"].toString());

	google->setAuthorizationUrl(authUri);
	google->setClientIdentifier(clientID);
	google->setAccessTokenUrl(tokenUri);
	google->setClientIdentifierSharedKey(clientSecret);
	
	auto replyHandler = new QOAuthHttpServerReplyHandler(this);
	google->setReplyHandler(replyHandler);
	google->grant();

}

QJsonDocument
googleDriveInterface::getCredentials(const QString fileName) {
	QFile loadFile(fileName);

	if (!loadFile.open(QIODevice::ReadOnly)) {
		qDebug() << "unable to open JSON file:" << loadFile.errorString();
	}

	QByteArray credentialsData = loadFile.readAll();
	QJsonDocument credentialsDoc(QJsonDocument::fromJson(credentialsData));
	loadFile.close();
	return credentialsDoc;
}