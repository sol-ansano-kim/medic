from ._qt import QtWidgets
from ._qt import QtCore
from ._qt import QtGui
import os
from . import model


IconDir = os.path.abspath(os.path.join(__file__, "../icons"))


class ListItemDelegate(QtWidgets.QStyledItemDelegate):
    def __init__(self, parent=None):
        super(ListItemDelegate, self).__init__(parent=parent)
        self.__background_color = QtGui.QColor(255, 255, 255)
        self.__alternate_background_color = QtGui.QColor(238, 238, 238)
        self.__selected_color = QtGui.QColor(30, 108, 210)

    def getBackgroudColor(self, option, index):
        if (option.state & QtWidgets.QStyle.State_Selected):
            return self.__selected_color
        elif (index.row() % 2 != 0):
            return self.__alternate_background_color
        return self.__background_color


class ReportDelegate(ListItemDelegate):
    def __init__(self, parent=None):
        super(ReportDelegate, self).__init__(parent=parent)

    def paint(self, painter, option, index):
        painter.save()
        rect = option.rect
        font_matrics = QtGui.QFontMetrics(option.font)
        report_name = index.data(model.DisplayRole)

        painter.fillRect(rect, self.getBackgroudColor(option, index))
        painter.drawText(rect.x() + 5, rect.y() + 15, report_name)

        painter.restore()


class KarteDelegate(ListItemDelegate):
    def __init__(self, parent=None):
        super(KarteDelegate, self).__init__(parent=parent)
        self.__karte_icon = QtGui.QPixmap(os.path.join(IconDir, "karte.png"))

    def paint(self, painter, option, index):
        painter.save()
        rect = option.rect
        font_matrics = QtGui.QFontMetrics(option.font)
        karte_name = index.data(model.DisplayRole)

        painter.fillRect(rect, self.getBackgroudColor(option, index))
        painter.drawPixmap(QtCore.QRect(rect.x() + 10, rect.y(), 50, 50), self.__karte_icon)
        painter.drawText(rect.x() + 80, rect.y() + rect.height() * 0.5 + font_matrics.height() * 0.5 - 4, karte_name)

        painter.restore()

    def sizeHint(self, option, index):
        return QtCore.QSize(option.decorationSize.width(), 50)


class TesterDelegate(ListItemDelegate):
    def __init__(self, parent=None):
        super(TesterDelegate, self).__init__(parent=parent)
        self.__ready_icon = QtGui.QPixmap(os.path.join(IconDir, "ready.png"))
        self.__success_icon = QtGui.QPixmap(os.path.join(IconDir, "success.png"))
        self.__failure_icon = QtGui.QPixmap(os.path.join(IconDir, "failure.png"))

    def paint(self, painter, option, index):
        painter.save()
        rect = option.rect
        font_matrics = QtGui.QFontMetrics(option.font)
        tester_name = index.data(model.DisplayRole)

        status = index.data(model.StatusRole)
        if status == model.Ready:
            icon = self.__ready_icon
        elif status == model.Success:
            icon = self.__success_icon
        elif status == model.Failure:
            icon = self.__failure_icon

        painter.fillRect(rect, self.getBackgroudColor(option, index))
        painter.drawPixmap(QtCore.QRect(rect.x() + 12, rect.y() + 2, 16, 16), icon)
        painter.drawText(rect.x() + 40, rect.y() + rect.height() * 0.5 + font_matrics.height() * 0.5 - 4, tester_name)

        painter.restore()

    def sizeHint(self, option, index):
        return QtCore.QSize(option.decorationSize.width(), 20)
