try:
    from PySide2 import QtWidgets
    from PySide2 import QtGui
    from PySide2 import QtCore
    from PySide2.QtWidgets import QAction
    import shiboken2 as shiboken
except:
    from PySide6 import QtWidgets
    from PySide6 import QtGui
    from PySide6 import QtCore
    from PySide6.QtGui import QAction
    import shiboken6 as shiboken