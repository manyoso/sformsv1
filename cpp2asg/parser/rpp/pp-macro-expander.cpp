/*
  Copyright 2005 Roberto Raggi <roberto@kdevelop.org>
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

#include "pp-macro-expander.h"

#include <QDate>
#include <QTime>

#include <qdebug.h>

#include "pp-internal.h"
#include "pp-engine.h"
#include "pp-environment.h"

using namespace rpp;

pp_frame::pp_frame(pp_macro* __expandingMacro, const QList<QString>& __actuals)
  : expandingMacro(__expandingMacro)
  , actuals(__actuals)
{
}

QString pp_macro_expander::resolve_formal(const QString& name)
{
  Q_ASSERT(!name.isEmpty());

  if (!m_frame)
    return QString();

  Q_ASSERT(m_frame->expandingMacro != 0);

  const QStringList& formals = m_frame->expandingMacro->formals;

  for (int index = 0; index < formals.size(); ++index) {
    if (name == formals[index]) {
      if (index < m_frame->actuals.size())
        return m_frame->actuals[index];
      else
        qWarning() << "Call to macro " << name << " missing argument number " << index << endl;
        // Triggers on deflate.c
        //Q_ASSERT(0); // internal error?
    }
  }

  return QString();
}

pp_macro_expander::pp_macro_expander(pp* engine, pp_frame* frame)
  : m_engine(engine)
  , m_frame(frame)
{
}

void pp_macro_expander::operator()(Stream& input, Stream& output)
{
  skip_blanks(input, output);

  while (!input.atEnd())
  {
    if (input == '\n')
    {
      output << input;

      skip_blanks(++input, output);

      if (!input.atEnd() && input == '#')
        break;
    }
    else if (input == '#')
    {
      skip_blanks(++input, output);

      QString identifier = skip_identifier(input);
      QString formal = resolve_formal(identifier);

      if (!formal.isEmpty()) {
        Stream is(&formal);
        skip_whitespaces(is, devnull());

        output << '\"';

        while (!is.atEnd()) {
          if (input == '"') {
            output << '\\' << is;

          } else if (input == '\n') {
            output << '"' << is << '"';

          } else {
            output << is;
          }

          skip_whitespaces(++is, output);
        }

        output << '\"';

      } else {
        output << '#'; // TODO ### warning message?
      }

    }
    else if (input == '\"')
    {
      skip_string_literal(input, output);
    }
    else if (input == '\'')
    {
      skip_char_literal(input, output);
    }
    else if (isComment(input))
    {
      skip_comment_or_divop(input, output);
    }
    else if (input.current().isSpace())
    {
      do {
        if (input == '\n' || !input.current().isSpace())
          break;

        output << input;

      } while (!(++input).atEnd());
    }
    else if (input.current().isNumber())
    {
      skip_number (input, output);
    }
    else if (input.current().isLetter() || input == '_')
    {
      QString name = skip_identifier (input);

      // search for the paste token
      int blankStart = input.pos();
      skip_blanks (input, devnull());
      if (!input.atEnd() && input == '#') {
        ++input;

        if (!input.atEnd() && input == '#')
          skip_blanks(++input, devnull());
        else
          input.seek(blankStart);

      } else {
        input.seek(blankStart);
      }

      Q_ASSERT(name.length() >= 0 && name.length() < 512);

      QString actual = resolve_formal(name);
      if (!actual.isEmpty()) {
        output << actual;
        continue;
      }

      // TODO handle inbuilt "defined" etc functions

      pp_macro* macro = m_engine->environment()->retrieveMacro(name);
      if (!macro || macro->hidden || m_engine->hideNextMacro())
      {
        m_engine->setHideNextMacro(name == "defined");

        if (name == "__LINE__")
          output << QString::number(input.inputLineNumber());
        else if (name == "__FILE__")
          output << '"' << m_engine->currentFile() << '"';
        else if (name == "__DATE__")
          output << QDate::currentDate().toString("MMM dd yyyy");
        else if (name == "__TIME__")
          output << QTime::currentTime().toString("hh:mm:ss");
        else
          output << name;
        continue;
      }

      if (!macro->function_like)
      {
        pp_macro* m = 0;

        if (!macro->definition.isEmpty()) {
          macro->hidden = true;

          pp_macro_expander expand_macro(m_engine);
          Stream ms(&macro->definition, QIODevice::ReadOnly);
          ms.setInputLineNumber(input.inputLineNumber());
          QString expanded;
          {
            Stream es(&expanded);
            expand_macro(ms, es);
          }

          if (!expanded.isEmpty())
          {
            Stream es(&expanded);
            skip_whitespaces(es, devnull());
            QString identifier = skip_identifier(es);

            pp_macro* m2 = 0;
            if (es.atEnd() && (m2 = m_engine->environment()->retrieveMacro(identifier))) {
              m = m2;

            } else {
              output << expanded;
            }
          }

          macro->hidden = false;
        }

        if (!m)
          continue;

        macro = m;
      }

      skip_whitespaces(input, devnull());

      // function like macro
      if (input.atEnd() || input != '(')
      {
        output << name;
        continue;
      }

      QList<QString> actuals;
      ++input; // skip '('

      pp_macro_expander expand_actual(m_engine, m_frame);

      int before = input.pos();
      {
        actual.clear();

        {
          Stream as(&actual);
          skip_argument_variadics(actuals, macro, input, as);
        }

        if (input.pos() != before)
        {
          QString newActual;
          {
            Stream as(&actual);
            as.setInputLineNumber(input.inputLineNumber());
            Stream nas(&newActual);
            expand_actual(as, nas);
          }
          actuals.append(newActual);
        }
      }

      // TODO: why separate from the above?
      while (!input.atEnd() && input == ',')
      {
        actual.clear();
        ++input; // skip ','

        {
          {
            Stream as(&actual);
            skip_argument_variadics(actuals, macro, input, as);
          }

          QString newActual;
          {
            Stream as(&actual);
            Stream nas(&newActual);
            expand_actual(as, nas);
          }
          actuals.append(newActual);
        }
      }

      //Q_ASSERT(!input.atEnd() && input == ')');

      ++input; // skip ')'

#if 0 // ### enable me
      assert ((macro->variadics && macro->formals.size () >= actuals.size ())
                  || macro->formals.size() == actuals.size());
#endif

      pp_frame frame(macro, actuals);
      pp_macro_expander expand_macro(m_engine, &frame);
      macro->hidden = true;
      Stream ms(&macro->definition, QIODevice::ReadOnly);
      ms.setInputLineNumber(input.inputLineNumber());
      expand_macro(ms, output);
      macro->hidden = false;

    } else {
      output << input;
      ++input;
    }
  }
}

void pp_macro_expander::skip_argument_variadics (const QList<QString>& __actuals, pp_macro *__macro, Stream& input, Stream& output)
{
  int first;

  do {
    first = input.pos();
    skip_argument(input, output);

  } while ( __macro->variadics
            && first != input.pos()
            && !input.atEnd()
            && input == '.'
            && (__actuals.size() + 1) == __macro->formals.size());
}

// kate: indent-width 2;
