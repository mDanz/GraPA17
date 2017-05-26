#pragma once
#include <QSplitter>
#include <QVBoxLayout>


class CameraModel;
class OpenGLWidget;
class ViewPortModel;

class ViewPortWidget : public QWidget
{
	Q_OBJECT;

public:
	explicit ViewPortWidget(QWidget *parent = nullptr);
	~ViewPortWidget();

	void setModel(ViewPortModel *model);

	OpenGLWidget* getCurrentWidget() const;
	CameraModel* getCurrentCamera() const;

public slots:
	void update() const;
	//void homePosition();

	void singleViewActivated();
	void dualViewActivated();
	void quadViewActivated();

	void selectPerspective();
	void selectFront();
	void selectLeft();
	void selectTop();


private:
	enum ViewPortLayout
	{
		SINGLE_VIEW = 1,
		DUAL_VIEW = 2,
		QUAD_VIEW = 4
	};

	void initializeOpenGLWidgets();
	void initializeLayout();
	void switchToView(ViewPortLayout view);
	void hideWidgets(ViewPortLayout currentType) const;
	void showWidgets(ViewPortLayout currentType) const;
	void select(OpenGLWidget *widget);

	ViewPortModel *m_model;

	QVBoxLayout *m_layout;
	QSplitter *m_quadView;
	QSplitter *m_topView;
	QSplitter *m_bottomView;

	OpenGLWidget *m_perspectiveWidget;
	OpenGLWidget *m_frontWidget;
	OpenGLWidget *m_leftWidget;
	OpenGLWidget *m_topWidget;
	OpenGLWidget *m_currentWidget;

	ViewPortLayout m_currentLayout;

private slots:
	void syncBottom() const;
	void syncTop() const;
};
