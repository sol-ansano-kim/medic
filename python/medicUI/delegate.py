from Qt import QtWidgets
from Qt import QtCore
from Qt import QtGui
import os
from . import model


IconDir = os.path.abspath(os.path.join(__file__, "../icons"))


class ListItemDelegate(QtWidgets.QStyledItemDelegate):
    def __init__(self, parent=None):
        super(ListItemDelegate, self).__init__(parent=parent)
        self.__background_color = QtGui.QColor(255, 255, 255)
        self.__alternate_background_color = QtGui.QColor(238, 238, 238)
        self.__selected_color = QtGui.QColor(30, 108, 210)

    def getBackgroudColor(self, option):
        if (option.state & QtWidgets.QStyle.State_Selected):
            return self.__selected_color
        elif (option.features & QtWidgets.QStyleOptionViewItem.Alternate):
            return self.__alternate_background_color
        return self.__background_color


class KarteDelegate(ListItemDelegate):
    def __init__(self, parent=None):
        super(KarteDelegate, self).__init__(parent=parent)
        self.__karte_icon = QtGui.QPixmap(os.path.join(IconDir, "karte.png"))

    def paint(self, painter, option, index):
        painter.save()
        rect = option.rect

        painter.fillRect(rect, self.getBackgroudColor(option))

        font_matrics = QtGui.QFontMetrics(option.font)
        font_matrics.height() / 2.0
        
        karte_name = index.data(model.DisplayRole)
        painter.drawPixmap(QtCore.QRect(rect.x() + 10, rect.y(), 50, 50), self.__karte_icon)
        painter.drawText(rect.x() + 80, rect.y() + rect.height() * 0.5 + font_matrics.height() * 0.5, karte_name)

        painter.restore()

    def sizeHint(self, option, index):
        return QtCore.QSize(option.decorationSize.width(), 50)


class TesterDelegate(ListItemDelegate):
    def __init__(self, parent=None):
        super(TesterDelegate, self).__init__(parent=parent)
        self.__success_icon = QtGui.QPixmap(os.path.join(IconDir, "success.png"))
        self.__failure_icon = QtGui.QPixmap(os.path.join(IconDir, "failure.png"))

    def paint(self, painter, option, index):
        painter.save()
        rect = option.rect

        painter.fillRect(rect, self.getBackgroudColor(option))

        font_matrics = QtGui.QFontMetrics(option.font)
        font_matrics.height() / 2.0
        
        tester_name = index.data(model.DisplayRole)
        status = index.data(model.StatusRole)
        if status is not model.Ready:
            icon = self.__success_icon
            if status is model.Failure:
                icon = self.__failure_icon
            painter.drawPixmap(QtCore.QRect(rect.x() + 10, rect.y(), 20, 20), icon)

        painter.drawText(rect.x() + 40, rect.y() + rect.height() * 0.5 + font_matrics.height() * 0.5, tester_name)

        painter.restore()

    def sizeHint(self, option, index):
        return QtCore.QSize(option.decorationSize.width(), 20)
