/*
 * This file is part of the Shashlik project
 *
 * Copyright (C) 2015 Dan Leinir Turthra Jensen <admin@leinir.dk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <stdio.h>

#include <QGuiApplication>
#include <QStringList>
#include <qcommandlineparser.h>
#include <qtimer.h>

#include <klocalizedstring.h>

#include "Controller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("shashlik-launcher");
    app.setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Controller interface for the Shashlik Android application launcher");
    QCommandLineOption helpArgument =  parser.addHelpOption();
    QCommandLineOption versionArgument = parser.addVersionOption();

    QCommandLineOption jarArgument("launchjar", i18n("Run the jar passed as an argument. This will install and run the jar, and launch any services required to do so."), "jarfile");
    parser.addOption(jarArgument);

    QCommandLineOption guiArgument("ui", i18n("Start the graphical UI. Default behaviour."));
    parser.addOption(guiArgument);

    QCommandLineOption zygoteArgument("zygote", i18n("Start zygote only"));
    parser.addOption(zygoteArgument);

    QCommandLineOption surfaceflingerArgument("surfaceflinger", i18n("Start surfaceflinger only"));
    parser.addOption(surfaceflingerArgument);

    QCommandLineOption servicemanagerArgument("servicemanager", i18n("Start the servicemanager only"));
    parser.addOption(servicemanagerArgument);

    QCommandLineOption startArgument("start", i18n("Start everything needed to launch applications (equivalent to running with --servicemanager, surfaceflinger and --zygote separately and in that order)"));
    parser.addOption(startArgument);
    QCommandLineOption stopArgument("stop", i18n("Stop everything needed to launch applications"));
    parser.addOption(stopArgument);
    QCommandLineOption restartArgument("restart", i18n("Stop and start all services (equivalent to running shashlik-controller first with --stop and then with --start)"));
    parser.addOption(restartArgument);

    parser.process(app);

    Controller* controller = new Controller(&app);
    if(parser.isSet(jarArgument)) {
        // do a thing with this thing...
    }
    else if(parser.isSet(startArgument)) {
        controller->start();
    }
    else if(parser.isSet(zygoteArgument)) {
        controller->startZygote();
    }
    else if(parser.isSet(surfaceflingerArgument)) {
        controller->startSurfaceflinger();
    }
    else if(parser.isSet(servicemanagerArgument)) {
        controller->startServicemanager();
    }
    else if(parser.isSet(restartArgument)) {
        controller->restart();
    }
    else if(parser.isSet(stopArgument)) {
        controller->stop();
        QTimer::singleShot(0, &app, SLOT(quit()));
    }
    else {
        // no need to check for guiArgument - that is what we will have left, or no arguments, which is the same thing
        printf("Do the thing!\n");
    }

    return app.exec();
}
