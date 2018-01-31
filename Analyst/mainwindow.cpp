/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

//! [0]
#include <QtWidgets>
#ifndef QT_NO_PRINTDIALOG
#include <QtPrintSupport>
#endif

#include "mainwindow.h"
//! [0]

//! [1]
MainWindow::MainWindow() {
  chartWidget = new QWidget(this);
  chartWidget->setObjectName("central widget");
  resize(800, 600);

  chartPloter = new QCustomPlot(chartWidget);
  chartPloter->setObjectName(QStringLiteral("chartWidget"));

  gridLayout = new QGridLayout(chartWidget);
  gridLayout->setSpacing(6);
  gridLayout->setContentsMargins(11, 11, 11, 11);
  gridLayout->setObjectName(QStringLiteral("gridLayout"));

  QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(chartPloter->sizePolicy().hasHeightForWidth());
  chartPloter->setSizePolicy(sizePolicy);

  gridLayout->addWidget(chartPloter, 0, 0, 1, 1);

  vScrollBar = new QScrollBar(chartWidget);
  vScrollBar->setObjectName(QStringLiteral("verticalScrollBar"));
  vScrollBar->setOrientation(Qt::Vertical);

  gridLayout->addWidget(vScrollBar, 0, 1, 1, 1);

  hScrollBar = new QScrollBar(chartWidget);
  hScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
  hScrollBar->setOrientation(Qt::Horizontal);

  gridLayout->addWidget(hScrollBar, 1, 0, 1, 1);

  setCentralWidget(chartWidget);

  createActions();
  createStatusBar();
  createDockWindows();

  setWindowTitle(tr("GoM Analyst"));
  // initialize axis range (and scroll bar positions via signals we just connected):
  chartPloter->xAxis->setRange(0, 600000, Qt::AlignCenter);
  chartPloter->yAxis->setRange(0, 100000, Qt::AlignCenter);

  newLetter();
  setUnifiedTitleAndToolBarOnMac(true);


  hScrollBar->setRange(-500, 500);
  vScrollBar->setRange(-500, 500);

  // create connection between axes and scroll bars:
  connect(hScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));
  connect(vScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
  connect(chartPloter->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
  connect(chartPloter->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
}
//! [1]

//! [2]
void MainWindow::newLetter() {
  chartPloter->legend->setVisible(true);

  // generate two sets of random walk data (one for candlestick and one for ohlc
  // chart):
  int n = 5000;
  std::vector<double> vecTime(n), value1(n), value2(n);
  QDateTime start = QDateTime(QDate(2014, 6, 11));
  start.setTimeSpec(Qt::UTC);
  double startTime = start.toTime_t();
  double binSize = 3600 * 24; // bin data in 1 day intervals
  vecTime[0] = startTime;
  value1[0] = 60;
  value2[0] = 20;
  qsrand(time(0));
  for (int i = 1; i < n; ++i) {
    vecTime[i] = startTime + 3600 * i;
    value1[i] = value1[i - 1] + (qrand() / (double)RAND_MAX - 0.5) * 10;
    value2[i] = value2[i - 1] + (qrand() / (double)RAND_MAX - 0.5) * 3;
  }

  // create candlestick chart:
  QCPFinancial *candlesticks =
      new QCPFinancial(chartPloter->xAxis, chartPloter->yAxis);
  candlesticks->setName("Candlestick");
  candlesticks->setChartStyle(QCPFinancial::csCandlestick);
  candlesticks->data()->set(
      QCPFinancial::timeSeriesToOhlc(vecTime, value2, binSize/3.0, startTime));
  candlesticks->setWidth(binSize * 0.2);
  candlesticks->setTwoColored(true);
  // candlesticks->setBrushPositive(QColor(245, 245, 245));
  // candlesticks->setBrushNegative(QColor(40, 40, 40));
  // candlesticks->setPenPositive(QPen(QColor(0, 0, 0)));
  // candlesticks->setPenNegative(QPen(QColor(0, 0, 0)));
  candlesticks->setBrushPositive(QColor(245, 0, 0));
  candlesticks->setBrushNegative(QColor(0, 245, 0));
  candlesticks->setPenPositive(QPen(QColor(255, 0, 0)));
  candlesticks->setPenNegative(QPen(QColor(0, 255, 0)));

  // create ohlc chart:
  // QCPFinancial *ohlc = new QCPFinancial(chartWidget->xAxis,
  // chartWidget->yAxis);
  // ohlc->setName("OHLC");
  // ohlc->setChartStyle(QCPFinancial::csOhlc);
  // ohlc->data()->set(QCPFinancial::timeSeriesToOhlc(time, value2, binSize/3.0,
  // startTime)); // divide binSize by 3 just to make the ohlc bars a bit denser
  // ohlc->setWidth(binSize*0.2);
  // ohlc->setTwoColored(true);

  // create bottom axis rect for volume bar chart:
  QCPAxisRect *volumeAxisRect = new QCPAxisRect(chartPloter);
  chartPloter->plotLayout()->addElement(1, 0, volumeAxisRect);
  volumeAxisRect->setMaximumSize(QSize(QWIDGETSIZE_MAX, 100));
  volumeAxisRect->axis(QCPAxis::atBottom)->setLayer("axes");
  volumeAxisRect->axis(QCPAxis::atBottom)->grid()->setLayer("grid");
  // bring bottom and main axis rect closer together:
  chartPloter->plotLayout()->setRowSpacing(0);
  volumeAxisRect->setAutoMargins(QCP::msLeft | QCP::msRight | QCP::msBottom);
  volumeAxisRect->setMargins(QMargins(0, 0, 0, 0));
  // create two bar plottables, for positive (green) and negative (red) volume
  // bars:
  chartPloter->setAutoAddPlottableToLegend(false);
  QCPBars *volumePos = new QCPBars(volumeAxisRect->axis(QCPAxis::atBottom),
                                   volumeAxisRect->axis(QCPAxis::atLeft));
  QCPBars *volumeNeg = new QCPBars(volumeAxisRect->axis(QCPAxis::atBottom),
                                   volumeAxisRect->axis(QCPAxis::atLeft));
  for (int i = 0; i < n / 5; ++i) {
    int v = qrand() % 20000 + qrand() % 20000 + qrand() % 20000 - 10000 * 3;
    (v < 0 ? volumeNeg : volumePos)
        ->addData(startTime + 3600 * 5.0 * i, qAbs(v)); // add data to either
                                                        // volumeNeg or
                                                        // volumePos, depending
                                                        // on sign of v
  }
  volumePos->setWidth(3600 * 4);
  volumePos->setPen(Qt::NoPen);
  volumePos->setBrush(QColor(100, 180, 110));
  volumeNeg->setWidth(3600 * 4);
  volumeNeg->setPen(Qt::NoPen);
  volumeNeg->setBrush(QColor(180, 90, 90));

  // interconnect x axis ranges of main and bottom axis rects:
  connect(chartPloter->xAxis, SIGNAL(rangeChanged(QCPRange)),
          volumeAxisRect->axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
  connect(volumeAxisRect->axis(QCPAxis::atBottom),
          SIGNAL(rangeChanged(QCPRange)), chartPloter->xAxis,
          SLOT(setRange(QCPRange)));
  // configure axes of both main and bottom axis rect:
  QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(
      new QCPAxisTickerDateTime);
  dateTimeTicker->setDateTimeSpec(Qt::UTC);
  dateTimeTicker->setDateTimeFormat("dd. MMMM");
  volumeAxisRect->axis(QCPAxis::atBottom)->setTicker(dateTimeTicker);
  volumeAxisRect->axis(QCPAxis::atBottom)->setTickLabelRotation(15);
  chartPloter->xAxis->setBasePen(Qt::NoPen);
  chartPloter->xAxis->setTickLabels(false);
  chartPloter->xAxis->setTicks(false); // only want vertical grid in main axis
                                       // rect, so hide xAxis backbone, ticks,
                                       // and labels
  chartPloter->xAxis->setTicker(dateTimeTicker);
  chartPloter->rescaleAxes();
  chartPloter->xAxis->scaleRange(1.025, chartPloter->xAxis->range().center());
  chartPloter->yAxis->scaleRange(1.1, chartPloter->yAxis->range().center());

  // make axis rects' left side line up:
  QCPMarginGroup *group = new QCPMarginGroup(chartPloter);
  chartPloter->axisRect()->setMarginGroup(QCP::msLeft | QCP::msRight, group);
  volumeAxisRect->setMarginGroup(QCP::msLeft | QCP::msRight, group);
}
//! [2]

//! [3]
void MainWindow::print() {
#ifndef QT_NO_PRINTDIALOG
#endif
}
//! [3]

//! [4]
void MainWindow::save() {}
//! [4]

//! [5]
void MainWindow::undo() {}
//! [5]

//! [6]
void MainWindow::drawChart(const QString &customer) {}
//! [6]

//! [7]
void MainWindow::addSomething(const QString &something) {}
//! [7]

void MainWindow::about() {
  QMessageBox::about(this, tr("About GoM Analyst"),
                     tr("The <b>GoM Analyst</b> example demonstrates how to "
                        "use Qt's dock widgets. You can enter your own text, "
                        "click a customer to add a customer name and "
                        "address, and click standard paragraphs to add them."));
}

void MainWindow::createActions() {
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  QToolBar *fileToolBar = addToolBar(tr("File"));

  const QIcon newIcon =
      QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
  QAction *newLetterAct = new QAction(newIcon, tr("&New Letter"), this);
  newLetterAct->setShortcuts(QKeySequence::New);
  newLetterAct->setStatusTip(tr("Create a new form letter"));
  connect(newLetterAct, &QAction::triggered, this, &MainWindow::newLetter);
  fileMenu->addAction(newLetterAct);
  fileToolBar->addAction(newLetterAct);

  const QIcon saveIcon =
      QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
  QAction *saveAct = new QAction(saveIcon, tr("&Save..."), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save the current form letter"));
  connect(saveAct, &QAction::triggered, this, &MainWindow::save);
  fileMenu->addAction(saveAct);
  fileToolBar->addAction(saveAct);

  const QIcon printIcon =
      QIcon::fromTheme("document-print", QIcon(":/images/print.png"));
  QAction *printAct = new QAction(printIcon, tr("&Print..."), this);
  printAct->setShortcuts(QKeySequence::Print);
  printAct->setStatusTip(tr("Print the current form letter"));
  connect(printAct, &QAction::triggered, this, &MainWindow::print);
  fileMenu->addAction(printAct);
  fileToolBar->addAction(printAct);

  fileMenu->addSeparator();

  QAction *quitAct = fileMenu->addAction(tr("&Quit"), this, &QWidget::close);
  quitAct->setShortcuts(QKeySequence::Quit);
  quitAct->setStatusTip(tr("Quit the application"));

  QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
  QToolBar *editToolBar = addToolBar(tr("Edit"));
  const QIcon undoIcon =
      QIcon::fromTheme("edit-undo", QIcon(":/images/undo.png"));
  QAction *undoAct = new QAction(undoIcon, tr("&Undo"), this);
  undoAct->setShortcuts(QKeySequence::Undo);
  undoAct->setStatusTip(tr("Undo the last editing action"));
  connect(undoAct, &QAction::triggered, this, &MainWindow::undo);
  editMenu->addAction(undoAct);
  editToolBar->addAction(undoAct);

  viewMenu = menuBar()->addMenu(tr("&View"));

  menuBar()->addSeparator();

  QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

  QAction *aboutAct =
      helpMenu->addAction(tr("&About"), this, &MainWindow::about);
  aboutAct->setStatusTip(tr("Show the application's About box"));

  QAction *aboutQtAct =
      helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

//! [8]
void MainWindow::createStatusBar() { statusBar()->showMessage(tr("Ready")); }
//! [8]

//! [9]
void MainWindow::createDockWindows() {
  QDockWidget *dock = new QDockWidget(tr("Customers"), this);
  dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  InstrumentList = new QListWidget(dock);
  InstrumentList->addItems(QStringList() << "rb888"
                                         << "ru888"
                                         << "i9888"
                                         << "j9888"
                                         << "m9888"
                                         << "ni888");
  dock->setWidget(InstrumentList);
  addDockWidget(Qt::RightDockWidgetArea, dock);
  viewMenu->addAction(dock->toggleViewAction());

  dock = new QDockWidget(tr("Paragraphs"), this);
  somethingList = new QListWidget(dock);
  dock->setWidget(somethingList);
  addDockWidget(Qt::RightDockWidgetArea, dock);
  viewMenu->addAction(dock->toggleViewAction());

  connect(InstrumentList, &QListWidget::currentTextChanged, this,
          &MainWindow::drawChart);
  connect(somethingList, &QListWidget::currentTextChanged, this,
          &MainWindow::addSomething);
}
//! [9]

void MainWindow::resizeEvent(QResizeEvent *event) {
  //  chartWidget->resize(600, 400);
  //    chartFrame->show();
  QWidget::resizeEvent(event);
}



void MainWindow::horzScrollBarChanged(int value)
{

  if (std::abs(chartPloter->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    chartPloter->xAxis->setRange(value/100.0, chartPloter->xAxis->range().size(), Qt::AlignCenter);
    chartPloter->replot();
  }
}

void MainWindow::vertScrollBarChanged(int value)
{
  if (qAbs(chartPloter->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    chartPloter->yAxis->setRange(-value/100.0, chartPloter->yAxis->range().size(), Qt::AlignCenter);
    chartPloter->replot();
  }
}

void MainWindow::xAxisChanged(QCPRange range)
{
  hScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
  hScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

void MainWindow::yAxisChanged(QCPRange range)
{
  vScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
  vScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

