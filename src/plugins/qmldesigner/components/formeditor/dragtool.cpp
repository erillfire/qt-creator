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

#include "dragtool.h"

#include "formeditorscene.h"
#include "formeditorview.h"
#include <metainfo.h>
#include <rewritingexception.h>

#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QMimeData>
#include <QTimer>
#include <QWidget>

namespace QmlDesigner {

namespace Internal {
void TimerHandler::clearMoveDelay()
{
    m_dragTool->clearMoveDelay();
}

}

DragTool::DragTool(FormEditorView *editorView)
    : AbstractFormEditorTool(editorView),
    m_moveManipulator(editorView->scene()->manipulatorLayerItem(), editorView),
    m_selectionIndicator(editorView->scene()->manipulatorLayerItem()),
    m_timerHandler(new Internal::TimerHandler(this)),
    m_blockMove(false),
    m_isAborted(false)
{
}


DragTool::~DragTool()
{
}

void DragTool::clear()
{
    m_moveManipulator.clear();
    m_selectionIndicator.clear();
    m_movingItem.clear();
}

void DragTool::mousePressEvent(const QList<QGraphicsItem*> &, QGraphicsSceneMouseEvent *)
{
}

void DragTool::mouseMoveEvent(const QList<QGraphicsItem*> &, QGraphicsSceneMouseEvent *)
{
}

void DragTool::hoverMoveEvent(const QList<QGraphicsItem*> &, QGraphicsSceneMouseEvent * /*event*/)
{
}

void DragTool::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        abort();
        event->accept();
        m_rewriterTransaction.commit();
        view()->changeToSelectionTool();
    }
}

void DragTool::keyReleaseEvent(QKeyEvent *)
{
}


void DragTool::mouseReleaseEvent(const QList<QGraphicsItem*> &/*itemList*/, QGraphicsSceneMouseEvent *)
{
}

void DragTool::mouseDoubleClickEvent(const QList<QGraphicsItem*> & /*itemList*/, QGraphicsSceneMouseEvent * /*event*/)
{
}

void DragTool::itemsAboutToRemoved(const QList<FormEditorItem*> & /* removedItemList */)
{
}

void DragTool::selectedItemsChanged(const QList<FormEditorItem*> &)
{
}



void DragTool::updateMoveManipulator()
{
}

void DragTool::beginWithPoint(const QPointF &beginPoint)
{
    m_movingItem = scene()->itemForQmlItemNode(m_dragNode);

    m_moveManipulator.setItem(m_movingItem.data());
    m_moveManipulator.begin(beginPoint);
}

void DragTool::createQmlItemNode(const ItemLibraryEntry &itemLibraryEntry,
                                 const QmlItemNode &parentNode,
                                 const QPointF &scenePos)
{
    MetaInfo metaInfo = MetaInfo::global();

    FormEditorItem *parentItem = scene()->itemForQmlItemNode(parentNode);
    QPointF pos = parentItem->mapFromScene(scenePos);

    m_dragNode = QmlItemNode::createQmlItemNode(view(), itemLibraryEntry, pos, parentNode);

    QList<QmlItemNode> nodeList;
    nodeList.append(m_dragNode);
    m_selectionIndicator.setItems(scene()->itemsForQmlItemNodes(nodeList));
}

void DragTool::createQmlItemNodeFromImage(const QString &imageName,
                                          const QmlItemNode &parentNode,
                                          const QPointF &scenePos)
{
    if (parentNode.isValid()) {
        MetaInfo metaInfo = MetaInfo::global();

        FormEditorItem *parentItem = scene()->itemForQmlItemNode(parentNode);
        QPointF pos = parentItem->mapFromScene(scenePos);

        m_dragNode = QmlItemNode::createQmlItemNodeFromImage(view(), imageName, pos, parentNode);

        QList<QmlItemNode> nodeList;
        nodeList.append(m_dragNode);
        m_selectionIndicator.setItems(scene()->itemsForQmlItemNodes(nodeList));
    }
}

static inline bool isAncestorOf(FormEditorItem *formEditorItem, FormEditorItem *newParentItem)
{
    if (formEditorItem && newParentItem)
        return formEditorItem->isAncestorOf(newParentItem);
    return false;
}

FormEditorItem* DragTool::targetContainerOrRootItem(const QList<QGraphicsItem*> &itemList, FormEditorItem * currentItem)
{
    FormEditorItem *formEditorItem = containerFormEditorItem(itemList, QList<FormEditorItem*>() << currentItem);

    if (!formEditorItem)
        formEditorItem = scene()->rootFormEditorItem();

    return formEditorItem;
}

void DragTool::formEditorItemsChanged(const QList<FormEditorItem*> & itemList)
{
    if (m_movingItem && itemList.contains(m_movingItem.data())) {
        QList<FormEditorItem*> updateItemList;
        updateItemList.append(m_movingItem.data());
        m_selectionIndicator.updateItems(updateItemList);
    }
}

void DragTool::instancesCompleted(const QList<FormEditorItem*> &itemList)
{
    m_moveManipulator.synchronizeInstanceParent(itemList);
    foreach (FormEditorItem* item, itemList)
        if (item->qmlItemNode() == m_dragNode)
            clearMoveDelay();
}

void DragTool::instancesParentChanged(const QList<FormEditorItem *> &itemList)
{
    m_moveManipulator.synchronizeInstanceParent(itemList);
}

void DragTool::instancePropertyChange(const QList<QPair<ModelNode, PropertyName> > & /*propertyList*/)
{
}

void DragTool::clearMoveDelay()
{
    if (m_blockMove) {
        m_blockMove = false;
        if (m_dragNode.isValid())
            beginWithPoint(m_startPoint);
    }
}

void DragTool::abort()
{
    if (!m_isAborted) {
        m_isAborted = true;

        if (m_dragNode.isValid())
            m_dragNode.destroy();
    }
}

static bool canHandleMimeData(const QMimeData *mimeData)
{
    return mimeData->hasFormat(QStringLiteral("application/vnd.bauhaus.itemlibraryinfo"))
          || mimeData->hasFormat(QStringLiteral("application/vnd.bauhaus.libraryresource"));
}

static bool hasItemLibraryInfo(const QMimeData *mimeData)
{
    return mimeData->hasFormat(QStringLiteral("application/vnd.bauhaus.itemlibraryinfo"));
}

static bool hasLibraryResources(const QMimeData *mimeData)
{
    return mimeData->hasFormat(QStringLiteral("application/vnd.bauhaus.libraryresource"));
}

void DragTool::dropEvent(const QList<QGraphicsItem*> &/*itemList*/, QGraphicsSceneDragDropEvent *event)
{
    if (canHandleMimeData(event->mimeData())) {
        event->accept();
        end(generateUseSnapping(event->modifiers()));

        m_rewriterTransaction.commit();

        if (m_dragNode.isValid()) {
            QList<QmlItemNode> nodeList;
            nodeList.append(m_dragNode);
            view()->setSelectedModelNodes(toModelNodeList(nodeList));
        }

        m_dragNode = QmlItemNode();

        view()->changeToSelectionTool();
    }
}

static ItemLibraryEntry itemLibraryEntryFromMimeData(const QMimeData *mimeData)
{
    QByteArray data = mimeData->data(QStringLiteral("application/vnd.bauhaus.itemlibraryinfo"));

    QDataStream stream(data);

    ItemLibraryEntry itemLibraryEntry;
    stream >> itemLibraryEntry;

    return itemLibraryEntry;
}

void DragTool::dragEnterEvent(const QList<QGraphicsItem*> &/*itemList*/, QGraphicsSceneDragDropEvent *event)
{
    if (canHandleMimeData(event->mimeData())) {
        m_blockMove = false;
        if (hasItemLibraryInfo(event->mimeData())) {
            view()->widgetInfo().widget->setFocus();
            m_isAborted = false;
        }

        if (!m_rewriterTransaction.isValid()) {
            view()->clearSelectedModelNodes();
            m_rewriterTransaction = view()->beginRewriterTransaction(QByteArrayLiteral("DragTool::dragEnterEvent"));
        }
    }
}

void DragTool::dragLeaveEvent(const QList<QGraphicsItem*> &/*itemList*/, QGraphicsSceneDragDropEvent *event)
{
    if (canHandleMimeData(event->mimeData())) {
        event->accept();

        m_moveManipulator.end();
        clear();
        if (m_dragNode.isValid())
            m_dragNode.destroy();

        m_rewriterTransaction.commit();

        view()->clearSelectedModelNodes();
        view()->changeToSelectionTool();
    }
}

static QString libraryResourceImageName(const QMimeData *mimeData)
{
   return QString::fromUtf8((mimeData->data(QStringLiteral("application/vnd.bauhaus.libraryresource"))));
}

void DragTool::createDragNode(const QMimeData *mimeData, const QPointF &scenePosition, const QList<QGraphicsItem*> &itemList)
{
    if (!m_dragNode.hasModelNode()) {
        FormEditorItem *targetContainerFormEditorItem = targetContainerOrRootItem(itemList);
        if (targetContainerFormEditorItem) {
            QmlItemNode targetContainerQmlItemNode;
            if (targetContainerFormEditorItem)
                targetContainerQmlItemNode = targetContainerFormEditorItem->qmlItemNode();

            if (hasItemLibraryInfo(mimeData))
                createQmlItemNode(itemLibraryEntryFromMimeData(mimeData), targetContainerQmlItemNode, scenePosition);
            else if (hasLibraryResources(mimeData))
                createQmlItemNodeFromImage(libraryResourceImageName(mimeData), targetContainerQmlItemNode, scenePosition);

            m_blockMove = true;
            m_startPoint = scenePosition;
            QTimer::singleShot(10000, m_timerHandler.data(), SLOT(clearMoveDelay()));
        }
    }
}

void DragTool::dragMoveEvent(const QList<QGraphicsItem*> &itemList, QGraphicsSceneDragDropEvent *event)
{
    if (!m_blockMove) {
        if (!m_isAborted) {
            if (canHandleMimeData(event->mimeData())) {
                event->accept();
                if (m_dragNode.isValid()) {
                    FormEditorItem *targetContainerItem = targetContainerOrRootItem(itemList);
                    if (targetContainerItem) {
                        move(event->scenePos(), itemList);
                    } else {
                        end();
                        m_dragNode.destroy();
                    }
                } else {
                    createDragNode(event->mimeData(), event->scenePos(), itemList);
                }
            }
        } else {
            event->ignore();
        }
    }
}

void  DragTool::end()
{
    m_moveManipulator.end();
    clear();
}

void DragTool::end(Snapper::Snapping useSnapping)
{
    m_moveManipulator.end(useSnapping);
    clear();
}

void  DragTool::move(const QPointF &scenePos, const QList<QGraphicsItem*> &itemList)
{
    if (m_movingItem) {
        FormEditorItem *containerItem = targetContainerOrRootItem(itemList, m_movingItem.data());
        if (containerItem && m_movingItem->parentItem() &&
                containerItem != m_movingItem->parentItem()) {

            m_moveManipulator.reparentTo(containerItem);
        }

        Snapper::Snapping useSnapping = Snapper::UseSnapping;

        m_moveManipulator.update(scenePos, useSnapping, MoveManipulator::UseBaseState);
    }
}

}
