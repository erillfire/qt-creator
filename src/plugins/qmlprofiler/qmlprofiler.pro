DEFINES += QMLPROFILER_LIBRARY

QT += network qml quick quickwidgets

include(../../qtcreatorplugin.pri)

SOURCES += \
    debugmessagesmodel.cpp \
    flamegraph.cpp \
    flamegraphmodel.cpp \
    flamegraphview.cpp \
    inputeventsmodel.cpp \
    localqmlprofilerrunner.cpp \
    memoryusagemodel.cpp \
    pixmapcachemodel.cpp \
    qmlprofileranimationsmodel.cpp \
    qmlprofilerattachdialog.cpp \
    qmlprofilerbindingloopsrenderpass.cpp \
    qmlprofilerclientmanager.cpp \
    qmlprofilerconfigwidget.cpp \
    qmlprofilerdatamodel.cpp \
    qmlprofilerdetailsrewriter.cpp \
    qmlprofilermodelmanager.cpp \
    qmlprofilernotesmodel.cpp \
    qmlprofileroptionspage.cpp \
    qmlprofilerplugin.cpp \
    qmlprofilerrangemodel.cpp \
    qmlprofilerrunconfigurationaspect.cpp \
    qmlprofilerruncontrol.cpp \
    qmlprofilerruncontrolfactory.cpp \
    qmlprofilersettings.cpp \
    qmlprofilerstatemanager.cpp \
    qmlprofilerstatewidget.cpp \
    qmlprofilerstatisticsmodel.cpp \
    qmlprofilerstatisticsview.cpp \
    qmlprofilertimelinemodel.cpp \
    qmlprofilertool.cpp \
    qmlprofilertraceclient.cpp \
    qmlprofilertracefile.cpp \
    qmlprofilertraceview.cpp \
    qmlprofilerviewmanager.cpp \
    scenegraphtimelinemodel.cpp

HEADERS += \
    debugmessagesmodel.h \
    flamegraph.h \
    flamegraphmodel.h \
    flamegraphview.h \
    inputeventsmodel.h \
    localqmlprofilerrunner.h \
    memoryusagemodel.h \
    pixmapcachemodel.h \
    qmlevent.h \
    qmleventtype.h \
    qmlnote.h \
    qmlprofiler_global.h \
    qmlprofileranimationsmodel.h \
    qmlprofilerattachdialog.h \
    qmlprofilerbindingloopsrenderpass.h \
    qmlprofilerclientmanager.h \
    qmlprofilerconfigwidget.h \
    qmlprofilerconstants.h \
    qmlprofilerdatamodel.h \
    qmlprofilerdetailsrewriter.h \
    qmlprofilereventlocation.h \
    qmlprofilereventsview.h \
    qmlprofilereventtypes.h \
    qmlprofilermodelmanager.h \
    qmlprofilernotesmodel.h \
    qmlprofileroptionspage.h \
    qmlprofilerplugin.h \
    qmlprofilerrangemodel.h \
    qmlprofilerrunconfigurationaspect.h \
    qmlprofilerruncontrol.h \
    qmlprofilerruncontrolfactory.h \
    qmlprofilersettings.h \
    qmlprofilerstatemanager.h \
    qmlprofilerstatewidget.h \
    qmlprofilerstatisticsmodel.h \
    qmlprofilerstatisticsview.h \
    qmlprofilertimelinemodel.h \
    qmlprofilertool.h \
    qmlprofilertraceclient.h \
    qmlprofilertracefile.h \
    qmlprofilertraceview.h \
    qmlprofilerviewmanager.h \
    scenegraphtimelinemodel.h

RESOURCES += \
    qml/qmlprofiler.qrc

FORMS += \
    qmlprofilerconfigwidget.ui
