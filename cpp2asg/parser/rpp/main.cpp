/*
  Copyright 2005 Roberto Raggi <roberto@kdevelop.org>

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

#include "pp-engine.h"

#include <kdebug.h>

#include "preprocessor.h"
#include "pp-environment.h"

using namespace rpp;

int main (int /*argc*/, char *argv[])
{
  bool no_stdinc = false;
  bool no_stdincpp = false;

  char const *filename = *++argv;
  if (!filename)
    {
      kDebug() << "usage: pp file.cpp" << endl;
      return EXIT_FAILURE;
    }

  Preprocessor p;

  QStringList ip;

  pp preprocess(&p);

  preprocess.processFile(QString("pp-configuration"), pp::File); // ### put your macros here!

  preprocess.environment()->enterBlock(0, QString());

  QString result = preprocess.processFile(QString(filename), pp::File);

  preprocess.environment()->cleanup();

  QStringList resultLines = result.split('\n');
  for (int i = 0; i < resultLines.count(); ++i)
    kDebug() << i << ": " << resultLines[i] << endl;

  /*foreach (const Preprocessor::MacroItem& macro, p.macros())
    kDebug() << "Macro '" << macro.name << "' param (" << macro.parameters << ") def (" << macro.definition << ") isFnLike (" << macro.isFunctionLike << ") filename (" << macro.fileName << ")" << endl;*/

  return EXIT_SUCCESS;
}

// kate: indent-width 2;
