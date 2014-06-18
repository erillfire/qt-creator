/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

import QtQuick 2.1

Item {
    id: labelContainer
    property string text: qmlProfilerModelProxy.title(modelIndex)
    property bool expanded: trigger(qmlProfilerModelProxy.expanded(modelIndex))
    property int modelIndex: index
    property int bindingTrigger: 1
    property var descriptions: []
    property var extdescriptions: []
    property var eventIds: []

    readonly property int dragHeight: 5

    function trigger(i) {
        return i * bindingTrigger * bindingTrigger;
    }

    visible: trigger(qmlProfilerModelProxy.rowCount(modelIndex)) > 0

    height: trigger(qmlProfilerModelProxy.height(modelIndex))
    width: 150

    function getDescriptions() {
        bindingTrigger = -bindingTrigger;
        backgroundMarks.requestPaint();
        if (!visible)
            return;

        var desc=[];
        var ids=[];
        var extdesc=[];
        var labelList = qmlProfilerModelProxy.getLabels(modelIndex);
        for (var i = 0; i < labelList.length; i++ ) {
            desc[i] = labelList[i].description;
            ids[i] = labelList[i].id;
            extdesc[i] = labelList[i].displayName + ":" + labelList[i].description;
        }
        descriptions = desc;
        eventIds = ids;
        extdescriptions = extdesc;
    }

    Connections {
        target: qmlProfilerModelProxy
        onExpandedChanged: getDescriptions();
        onStateChanged: getDescriptions()
        onRowHeightChanged: getDescriptions()
    }

    Text {
        id: txt
        x: 5
        font.pixelSize: 12
        text: labelContainer.text
        color: "#232323"
        height: trigger(qmlProfilerModelProxy.rowHeight(modelIndex, 0))
        width: 140
        verticalAlignment: Text.AlignVCenter
    }

    Rectangle {
        height: 1
        width: parent.width
        color: "#999999"
        anchors.bottom: parent.bottom
        z: 2
    }

    Column {
        anchors.top: txt.bottom
        visible: expanded
        Repeater {
            model: descriptions.length
            Rectangle {
                width: labelContainer.width
                height: trigger(qmlProfilerModelProxy.rowHeight(modelIndex, index + 1))
                color: "#eaeaea"
                border.width: 1
                border.color:"#c8c8c8"
                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 5
                    anchors.rightMargin: 5

                    text: descriptions[index]
                    textFormat: Text.PlainText
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }
                MouseArea {
                    property bool resizing: false
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: (resizing || height - mouseY < dragHeight) ? Qt.SizeVerCursor :
                                                                              Qt.ArrowCursor;
                    onEntered: changeToolTip(extdescriptions[index]);
                    onExited: changeToolTip("");
                    onPressed: resizing = (height - mouseY < dragHeight);

                    onReleased: resizing = false;

                    onClicked: {
                        if (mouse.modifiers & Qt.ShiftModifier)
                            view.selectPrevFromId(modelIndex,eventIds[index]);
                        else
                            view.selectNextFromId(modelIndex,eventIds[index]);
                    }

                    onMouseYChanged: {
                        if (resizing)
                            qmlProfilerModelProxy.setRowHeight(modelIndex, index + 1, mouseY);
                    }
                }
            }
        }
    }

    Image {
        source: expanded ? "arrow_down.png" : "arrow_right.png"
        x: parent.width - 12
        y: 9
        smooth: false
        MouseArea {
            anchors.fill: parent
            anchors.rightMargin: -10
            anchors.leftMargin: -10
            anchors.topMargin: -10
            anchors.bottomMargin: -10
            onClicked: {
                // Don't try to expand empty models.
                if (expanded || qmlProfilerModelProxy.count(modelIndex) > 0)
                    qmlProfilerModelProxy.setExpanded(modelIndex, !expanded);
            }
        }
    }
}
