//
//
//#ifndef WINDOW_H
//#define WINDOW_H
//
//#include <QWidget>
//
//class QSlider;
//class QPushButton;
//
//class GLRenderingWidget;
//class MainWindow;
//
//class Window : public QWidget
//{
//	Q_OBJECT
//
//public:
//	explicit Window(MainWindow *mw);
//
//protected:
//	void keyPressEvent(QKeyEvent *event) override;
//
//private slots:
//	void dockUndock();
//
//private:
//	QSlider *createSlider();
//
//	GLRenderingWidget *glWidget;
//	QSlider *xSlider;
//	QSlider *ySlider;
//	QSlider *zSlider;
//	QPushButton *dockBtn;
//	MainWindow *mainWindow;
//};
//
//#endif