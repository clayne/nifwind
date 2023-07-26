//  "The Last Hunt begins, Young Bull! Hopper screamed. We live. We live!"

/**** BEGIN LICENSE BLOCK ****

BSD 3-Clause License

Copyright (c) 2023, the wind.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**** END LICENCE BLOCK ****/

#include "MainWindow.h"

#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QApplication>
#include <QFileDialog>
#include <QTreeView>
#include <QDockWidget>
#include <QFileSystemModel>
#include <QVector>
#include <QString>

#include "TreeModel.h"

namespace nifwind {

struct BigBadNode
{
    BigBadNode * Base{};
    int Index; // Because QAbstractItemModel::parent()
    QVector<BigBadNode *> Nodes {};
    inline int Count() { return Nodes.size (); }
    inline BigBadNode * operator[](int i) { return Nodes[i]; }
    QString Name {"root doesn't drink bonbons"};
    BigBadNode(BigBadNode * b = nullptr)
    {
        if (b)
            Base = b, b->Nodes.push_back (this);
    }
    ~BigBadNode() { for (auto f : Nodes) delete f; }
};

MainWindow::MainWindow()
    : QMainWindow {}
{
    // main menu
    auto miFile = menuBar ()->addMenu ("&File");
        miFile->addAction ("&Open", this, &MainWindow::HandleFileOpen);
        miFile->addSeparator ();

        auto aQuit = miFile->addAction ("&Quit");
        connect (aQuit, &QAction::triggered, this, &MainWindow::close);

    auto miHelp = menuBar ()->addMenu ("&Help");
        miHelp->addAction ("&About Qt", qApp, &QApplication::aboutQt);

    //
    auto root = new BigBadNode; // can't be shown
    new BigBadNode {root};
    auto tree = new TreeModel<BigBadNode> (root, this);
    // auto tree = new QFileSystemModel {this};
    // tree->setRootPath (QDir::currentPath ());
    auto tv = new QTreeView {};
    // I have to sub-class QTreeView just to set its initial size as a docked
    // tree?! You have got to be nice mountain view kidding me. TODO read
    // QDockWidget
    tv->setModel (tree);
    auto foo = new QDockWidget {"TreeView"};
    foo->setWidget (tv);
    addDockWidget (Qt::LeftDockWidgetArea, foo);
    // status bar
    statusBar ()->showMessage ("Idle");
}

MainWindow::~MainWindow() { printf ("I am defined\n"); }

void MainWindow::HandleFileOpen()
{
    QFileDialog dlg {this};
    dlg.setFileMode (QFileDialog::ExistingFile);
    dlg.setNameFilter ("Nif files (*.nif *.nifcache *.texcache)");
    dlg.setViewMode (QFileDialog::Detail);
    if (dlg.exec ()) printf ("Open it\n");
    else printf ("Don't open it\n");
}

}
