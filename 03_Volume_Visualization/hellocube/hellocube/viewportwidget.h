#pragma once
#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>


class OpenGLWidget;
class ViewPortModel;

class ViewPortWidget : QWidget
{
	Q_OBJECT;

public:
	ViewPortWidget();
	~ViewPortWidget();

	void setModel(ViewPortModel *model) const;

	OpenGLWidget* getCurrentWidget() const;

public slots:
	void update() const;
	//void homePosition();

	//void singleView();
	//void dualView();
	//void quadView();

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
