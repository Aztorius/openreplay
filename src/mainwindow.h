/* ****************************************************************************
 *
 * Copyright 2016 William Bonnaventure
 *
 * This file is part of LegendsReplay.
 *
 * LegendsReplay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LegendsReplay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LegendsReplay.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ****************************************************************************
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QtNetwork>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QMenu>
#include <QAction>
#include <QListWidgetItem>
#include <QProgressBar>
#include <QFileSystemWatcher>
#include <QtGlobal>
#include <QHash>
#include <QPair>

#include "qhttpserver.hpp"
#include "qhttpserverresponse.hpp"
#include "qhttpserverrequest.hpp"

#include "replay.h"
#include "recorder.h"
#include "server.h"
#include "gameinfoswidget.h"
#include "advancedrecorderdialog.h"
#include "repairtooldialog.h"

using namespace qhttp::server;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setArgs(int argc, char *argv[]);

    void lol_launch(QString platformid, QString key, QString matchid, bool local = false);
    bool check_path(QString path);
    bool game_ended(QString region, QString gameid);
    void replay_launch(QString pathfile);
    void rofl_file_launch(QString filepath);

    QJsonDocument getCurrentPlayingGameInfos(QString serverRegion, QString summonerId);
    QJsonDocument getJsonFromUrl(QString url);
    QByteArray getFileFromUrl(QString url);

    QPixmap getImg(int id);
    bool islolRunning();
    bool islolclientRunning();

    Server getServerByPlatformId(QString platformid);
    Server getServerByRegion(QString region);
    Server getServerByDomain(QString domain);

    void refreshRecordingGamesWidget();

private:
    Ui::MainWindow *ui;

    QSettings *lrsettings;

    QNetworkAccessManager *networkManager_status;
    QNetworkAccessManager *networkManager_featured;
    QNetworkAccessManager *networkManager_replayServers;

    QSystemTrayIcon *systemtrayicon;
    QMenu *systemtraymenu;

    QMenu *custommenu;

    /* Contains the game id of all recording games.
     * Key: (QString platformId, QString gameId)
     * Value: (QString dateTime, QThread* pointerToRecordingThread)
     */
    QHash <QPair<QString, QString>, QPair <QString, QThread*>> recording;

    QList <QString> recordedgames_filename;
    QList <QString> yourgames_filename;

    QList <QJsonObject> json_status;
    QList <QJsonObject> json_featured;

    QList <Server> servers;
    QList <QString> lrservers;

    QString loldirectory;
    QString lolpbedirectory;
    QString replaydirectory;

    bool replaying;
    bool playing;
    bool systemtrayavailable;

    QString m_summonername;
    QString m_summonerid;
    QString m_summonerServerRegion;
    QString m_PBEname;
    QString m_PBEid;

    QString m_currentLegendsReplayServer;

    QTimer *m_timer;

    QHttpServer *httpserver;
    Replay *replay;

    QJsonDocument m_searchsummoner_game;

    int serverChunkCount;
    int serverKeyframeCount;

    QString m_currentsystemtraymessage;

    QTranslator translator;

    QFileSystemWatcher* m_directory_watcher;

private slots:
    void slot_checkSoftwareVersion();

    void slot_networkResult_status(QNetworkReply *reply);
    void slot_networkResult_featured(QNetworkReply *reply);
    void slot_networkResult_replayServers(QNetworkReply *reply);

    void slot_refreshPlayingStatus();
    void slot_refresh_recordedGames();

    void slot_doubleclick_savedgames(int row);
    void slot_doubleclick_mygames(int row);
    void slot_doubleclick_featured(QListWidgetItem*item);
    void slot_click_allgames();
    void slot_click_searchsummoner_spectate();
    void slot_click_searchsummoner_record();
    void slot_click_replayservers();

    void slot_searchsummoner();

    void slot_open_replay();
    void slot_launch_rofl_file();

    void slot_replayserversAdd();

    void slot_summonerinfos_save();

    void slot_featuredRefresh();
    void slot_statusRefresh();

    void slot_setdirectory();
    void slot_setreplaydirectory();

    void slot_featuredLaunch();
    void slot_featuredRecord();

    void slot_changedTab(int index);
    void slot_endRecording(QString platformid, QString gameid);
    void log(QString);
    void showmessage(QString message);
    void systemtrayiconActivated(QSystemTrayIcon::ActivationReason reason);
    void slot_messageclicked();

    void slot_openAdvancedRecorder();
    void slot_customGameRecord(QString serverAddress, QString serverRegion, QString gameId, QString encryptionKey, bool forceCompleteDownload = false, bool downloadInfos = false, bool downloadStats = true);

    void slot_customcontextmenu();

    void slot_reportAnIssue();
    void slot_aboutLegendsReplay();
    void slot_checkandrepair();

    void slot_setLanguage();

    void slot_menu_replay();
    void slot_menu_stats();
    void slot_menu_repairtool();
    void slot_menu_delete();
    void slot_menu_spectate();
    void slot_menu_record();
    void slot_menu_cancel();
    void slot_menu_cancelanddelete();

protected:
    void changeEvent(QEvent*);

signals:
    void signal_refresh_recordedGames();
    void signal_checkSoftwareVersion();
    void signal_refreshStatusServers();
};

#endif // MAINWINDOW_H
