/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2006
 *
 *  Last modified:
 *     $Date$ by $Author$
 *     $Revision$
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <QtGui>

#include <iostream>
#include <gecode/gist/textoutput.hh>
#include <gecode/gist/gecodelogo.hh>

namespace Gecode { namespace Gist {

  /// \brief An outputstream that prints on a QTextEdit
  class GistOutputStream
  : public std::basic_ostream<char, std::char_traits<char> > {
    /// \brief Buffer for printing on a QTextEdit
    class Buf
    : public std::basic_streambuf<char, std::char_traits<char> > {
      QString buffer;
      QTextEdit* editor;
    protected:
      virtual int overflow(int v = std::char_traits<char>::eof()) {
        if (v == '\n') {
          editor->append(buffer);
          buffer.clear();
        } else {
          buffer += (char)v;
        }
        return v;
      }
    public:
      Buf(QTextEdit* e) : editor(e) {}
    };

    Buf _buf;
  public:
    GistOutputStream(QTextEdit* editor)
    : std::basic_ostream<char, std::char_traits<char> >(&_buf),
      _buf(editor) {
      clear();
    }
  };

  TextOutput::TextOutput(const std::string& name, QWidget *parent)
  : QMainWindow(parent) {
    Logos logos;

    QPixmap myPic;
    myPic.loadFromData(logos.gistLogo, logos.gistLogoSize);
    setWindowIcon(myPic);

    QFont font;
    QString fontFamily("Courier");
    font.setFamily(fontFamily);
    font.setFixedPitch(true);
    font.setPointSize(12);

    editor = new QTextEdit;
    editor->setFont(font);
    editor->setReadOnly(true);
    editor->setLineWrapMode(QTextEdit::FixedColumnWidth);
    editor->setLineWrapColumnOrWidth(80);
    editor->setTabStopWidth(2);
    os = new GistOutputStream(editor);

    QAction* clearText = new QAction("Clear", this);
    clearText->setShortcut(QKeySequence("Ctrl+K"));
    this->addAction(clearText);
    connect(clearText, SIGNAL(triggered()), editor,
                       SLOT(clear()));

    QAction* closeWindow = new QAction("Close window", this);
    closeWindow->setShortcut(QKeySequence("Ctrl+W"));
    this->addAction(closeWindow);
    connect(closeWindow, SIGNAL(triggered()), this,
                         SLOT(close()));

    setCentralWidget(editor);
    setWindowTitle(QString((std::string("Gist Console: ") + name).c_str()));

    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, false);
    resize(600,300);
  }

  TextOutput::~TextOutput(void) {
    delete os;
  }

  std::ostream&
  TextOutput::getStream(void) {
    return *os;
  }

  void
  TextOutput::insertHtml(const QString& s) {
    editor->insertHtml(s);
  }

}}

// STATISTICS: gist-any
