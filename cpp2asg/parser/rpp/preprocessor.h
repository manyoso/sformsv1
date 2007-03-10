/*
  Copyright 2005 Harald Fernengel <harry@kdevelop.org>
  Copyright 2006 Hamish Rodda <rodda@kde.org>

  Permission to use, copy, modify, distribute, and sell this software and its
  documentation for any purpose is hereby granted without fee, provided that
  the above copyright notice appear in all copies and that both that
  copyright notice and this permission notice appear in supporting
  documentation.

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
  KDEVELOP TEAM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
  AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <QtCore/QString>
namespace rpp {

class Stream;
class Environment;

class Preprocessor
{
  friend class pp;
public:
    enum IncludeType {
      /// An include specified as being local (eg. "file.h")
      IncludeLocal,
      /// An include specified as being global (eg. &lt;file.h&gt;)
      IncludeGlobal
    };

    virtual ~Preprocessor();

    Environment *processString(const QString &fileName, const QString& string, QString *contents);

protected:
    /**
     * This function is called by the preprocessor whenever
     * it encounters an include directive.
     *
     * This class is permitted to modify \a fileName%; this
     * value will be used when marking the file in the preprocessed
     * output.
     *
     * \param fileName name of the source file to include
     * \param type the way that the file was requested
     *
     * \return a Stream with the appropriate contents to allow
     * the file to be #included.  Ownership of the Stream is yielded to
     * class pp at this point.
     */
    virtual Stream* sourceNeeded(QString& fileName, IncludeType type, int sourceLine);
};

}

#endif
