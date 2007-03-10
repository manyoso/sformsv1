#include "tosource.h"

#include <QDebug>

#include "use.h"
#include "ducontext.h"
#include "definition.h"
#include "declaration.h"
#include "cppeditorintegrator.h"
#include "parsesession.h"
#include "pp-environment.h"
#include "pp-macro.h"

static char const * const names[] = {
  0,
  "AccessSpecifier",
  "AsmDeclaration",
  "BaseClause",
  "BaseSpecifier",
  "BinaryExpression",
  "CastExpression",
  "ClassMemberAccess",
  "ClassSpecifier",
  "CompoundStatement",
  "Condition",
  "ConditionalExpression",
  "CppCastExpression",
  "CtorInitializer",
  "DeclarationStatement",
  "Declarator",
  "DeleteExpression",
  "DoStatement",
  "ElaboratedTypeSpecifier",
  "EnumSpecifier",
  "Enumerator",
  "ExceptionSpecification",
  "ExpressionOrDeclarationStatement",
  "ExpressionStatement",
  "ForStatement",
  "FunctionCall",
  "FunctionDeclaration",
  "IfStatement",
  "IncrDecrExpression",
  "InitDeclarator",
  "Initializer",
  "InitializerClause",
  "LabeledStatement",
  "LinkageBody",
  "LinkageSpecification",
  "MemInitializer",
  "Name",
  "Namespace",
  "NamespaceAliasDeclaration",
  "NewDeclarator",
  "NewExpression",
  "NewInitializer",
  "NewTypeId",
  "Operator",
  "OperatorFunctionId",
  "ParameterDeclaration",
  "ParameterDeclarationClause",
  "PostfixExpression",
  "PrimaryExpression",
  "PtrOperator",
  "PtrToMember",
  "ReturnStatement",
  "SimpleDeclaration",
  "SimpleTypeSpecifier",
  "SizeofExpression",
  "StringLiteral",
  "SubscriptExpression",
  "SwitchStatement",
  "TemplateArgument",
  "TemplateDeclaration",
  "TemplateParameter",
  "ThrowExpression",
  "TranslationUnit",
  "TryBlockStatement",
  "TypeId",
  "TypeIdentification",
  "TypeParameter",
  "Typedef",
  "UnaryExpression",
  "UnqualifiedName",
  "Using",
  "UsingDirective",
  "WhileStatement",
  "WinDeclSpec",
  "StatementName"
};

ToSource::ToSource(const QUrl &url)
    : m_url(url), m_debug(false), m_session(0), m_editor(0) {}

ToSource::~ ToSource() {
  delete m_editor;
}

QString ToSource::dump( AST * node, ParseSession* session) {
  delete m_editor;
  m_editor = 0;

  delete m_session;
  m_session = 0;

  m_session = session;
  m_editor = new CppEditorIntegrator(m_session);

  visit(node);

  QStringList list;
  foreach (QString *str, m_strings) {
    list.append(*str);
  }

//   rpp::Environment *e = session->environment;
//   QList<rpp::pp_macro*> macros = e->allMacros();
//   foreach (rpp::pp_macro *m, macros) {
//     list.append( "*** MACRO BEGIN ***" );
//     list.append( m->name );
//     list.append( m->definition );
//     list.append( m->file );
//     list.append( QString::number(m->sourceLine) );
//     list = list + m->formals;
// 
//     list.append( m->defined ? "true" : "false" );
//     list.append( m->hidden ? "true" : "false" );
//     list.append( m->function_like ? "true" : "false" );
//     list.append( m->variadics ? "true" : "false" );
//     list.append( "*** MACRO END ***" );
//   }

  return list.join("\n");
}

void ToSource::appendToken(std::size_t token) {

  const Token& t = m_session->token_stream->token(token);

  QString str = m_editor->tokenToString(token);

  int l, c;
  QString f;

  m_session->positionAt(t.position, &l, &c, &f);

  QUrl url = QUrl::fromLocalFile(f);

  if (url != m_url && !f.isEmpty() ) {
    //qDebug() << url << " VS " << m_url << endl;
    return;
  }

  int grow = abs(m_strings.count() - l - 1);
  /*  qDebug() << grow << endl;*/
  if (m_strings.count() - 1 < l)
    for (int i = 0; i < grow; ++i)
      m_strings.append(new QString());

  QString *line = m_strings.at(l);
  Q_ASSERT(line);

  line->replace(c, str.length(), str);
//   qDebug() << *line << endl;
}

void ToSource::visit(AST *node) {

  if (!node || !m_editor) {
    DefaultVisitor::visit(node);
    return;
  }

  Range r = m_editor->findRange(node);

  QString startAST = m_editor->tokenToString(node->start_token);
  QString endAST = m_editor->tokenToString(node->end_token);

  if (m_debug)
  qDebug() << "start" << names[node->kind] /*<< " " */<< r.start() /*<< " "*/ << startAST
           << " AND "
           << "end" << names[node->kind] << " " << r.end() /*<< " "*/ << endAST << endl;

  appendToken(node->start_token);
  appendToken(node->end_token);

  DefaultVisitor::visit(node);
}

void ToSource::visitAccessSpecifier(AccessSpecifierAST *node)
{
  appendToken(node->colon);
  appendTokens(this, node->specs);
  DefaultVisitor::visitAccessSpecifier(node);
}

void ToSource::visitAsmDefinition(AsmDefinitionAST *node)
{
  appendTokens(this, node->cv);
  DefaultVisitor::visitAsmDefinition(node);
}

// void ToSource::visitBaseClause(BaseClauseAST *node)
// {
// }

void ToSource::visitBaseSpecifier(BaseSpecifierAST *node)
{
  appendToken(node->virt);
  appendToken(node->access_specifier);
  DefaultVisitor::visitBaseSpecifier(node);
}

void ToSource::visitBinaryExpression(BinaryExpressionAST *node)
{
  appendToken(node->op);
  DefaultVisitor::visitBinaryExpression(node);
}

// void ToSource::visitCastExpression(CastExpressionAST *node)
// {
// }

void ToSource::visitClassMemberAccess(ClassMemberAccessAST *node)
{
  appendToken(node->op);
  DefaultVisitor::visitClassMemberAccess(node);
}

void ToSource::visitClassSpecifier(ClassSpecifierAST *node)
{
  appendToken(node->class_key);
  appendToken(node->end_bracket);
  DefaultVisitor::visitClassSpecifier(node);
}

// void ToSource::visitCompoundStatement(CompoundStatementAST *node)
// {
// }
// 
// void ToSource::visitCondition(ConditionAST *node)
// {
// }
// 
// void ToSource::visitConditionalExpression(ConditionalExpressionAST *node)
// {
// }

void ToSource::visitCppCastExpression(CppCastExpressionAST *node)
{
  appendToken(node->op);
  DefaultVisitor::visitCppCastExpression(node);
}

void ToSource::visitCtorInitializer(CtorInitializerAST *node)
{
  appendToken(node->colon);
  DefaultVisitor::visitCtorInitializer(node);
}

// void ToSource::visitDeclarationStatement(DeclarationStatementAST *node)
// {
// }

void ToSource::visitDeclarator(DeclaratorAST *node)
{
  appendTokens(this, node->fun_cv);
  DefaultVisitor::visitDeclarator(node);
}

void ToSource::visitDeleteExpression(DeleteExpressionAST *node)
{
  appendToken(node->scope_token);
  appendToken(node->delete_token);
  appendToken(node->lbracket_token);
  appendToken(node->rbracket_token);
  DefaultVisitor::visitDeleteExpression(node);
}

// void ToSource::visitDoStatement(DoStatementAST *node)
// {
// }

void ToSource::visitElaboratedTypeSpecifier(ElaboratedTypeSpecifierAST *node)
{
  appendTokens(this, node->cv);
  appendToken(node->type);
  DefaultVisitor::visitElaboratedTypeSpecifier(node);
}

// void ToSource::visitEnumSpecifier(EnumSpecifierAST *node)
// {
// }

void ToSource::visitEnumerator(EnumeratorAST *node)
{
  appendToken(node->id);
  DefaultVisitor::visitEnumerator(node);
}

void ToSource::visitExceptionSpecification(ExceptionSpecificationAST *node)
{
  appendToken(node->ellipsis);
  DefaultVisitor::visitExceptionSpecification(node);
}

// void ToSource::visitExpressionOrDeclarationStatement(ExpressionOrDeclarationStatementAST *node)
// {
// }
// 
// void ToSource::visitExpressionStatement(ExpressionStatementAST *node)
// {
// }
// 
// void ToSource::visitForStatement(ForStatementAST *node)
// {
// }

void ToSource::visitFunctionCall(FunctionCallAST *node)
{
  appendToken(node->right_paren);
  DefaultVisitor::visitFunctionCall(node);
}

void ToSource::visitFunctionDefinition(FunctionDefinitionAST *node)
{
  appendTokens(this, node->storage_specifiers);
  appendTokens(this, node->function_specifiers);
  DefaultVisitor::visitFunctionDefinition(node);
}

// void ToSource::visitIfStatement(IfStatementAST *node)
// {
// }

void ToSource::visitIncrDecrExpression(IncrDecrExpressionAST *node)
{
  appendToken(node->op);
  DefaultVisitor::visitIncrDecrExpression(node);
}

// void ToSource::visitInitDeclarator(InitDeclaratorAST *node)
// {
// }
// 
// void ToSource::visitInitializer(InitializerAST *node)
// {
// }
// 
// void ToSource::visitInitializerClause(InitializerClauseAST *node)
// {
// }
// 
// void ToSource::visitLabeledStatement(LabeledStatementAST *node)
// {
// }
// 
// void ToSource::visitLinkageBody(LinkageBodyAST *node)
// {
// }

void ToSource::visitLinkageSpecification(LinkageSpecificationAST *node)
{
  appendToken(node->extern_type);
  DefaultVisitor::visitLinkageSpecification(node);
}

void ToSource::visitMemInitializer(MemInitializerAST *node)
{
  appendToken(node->right_paren);
  DefaultVisitor::visitMemInitializer(node);
}

// void ToSource::visitName(NameAST *node)
// {
// }

void ToSource::visitNamespace(NamespaceAST *node)
{
  appendToken(node->namespace_name);
  DefaultVisitor::visitNamespace(node);
}

void ToSource::visitNamespaceAliasDefinition(NamespaceAliasDefinitionAST *node)
{
  appendToken(node->namespace_name);
  DefaultVisitor::visitNamespaceAliasDefinition(node);
}

// void ToSource::visitNewDeclarator(NewDeclaratorAST *node)
// {
// }

void ToSource::visitNewExpression(NewExpressionAST *node)
{
  appendToken(node->scope_token);
  appendToken(node->new_token);
  DefaultVisitor::visitNewExpression(node);
}

void ToSource::visitNewInitializer(NewInitializerAST *node)
{
  appendToken(node->right_paren);
  DefaultVisitor::visitNewInitializer(node);
}

// void ToSource::visitNewTypeId(NewTypeIdAST *node)
// {
// }

void ToSource::visitOperator(OperatorAST *node)
{
  appendToken(node->op);
  appendToken(node->open);
  appendToken(node->close);
  DefaultVisitor::visitOperator(node);
}

// void ToSource::visitOperatorFunctionId(OperatorFunctionIdAST *node)
// {
// }
// 
// void ToSource::visitParameterDeclaration(ParameterDeclarationAST *node)
// {
// }

void ToSource::visitParameterDeclarationClause(ParameterDeclarationClauseAST *node)
{
  appendToken(node->ellipsis);
  DefaultVisitor::visitParameterDeclarationClause(node);
}

void ToSource::visitPostfixExpression(PostfixExpressionAST *node)
{
  DefaultVisitor::visitPostfixExpression(node);
}

void ToSource::visitPrimaryExpression(PrimaryExpressionAST *node)
{
  appendToken(node->token);
  DefaultVisitor::visitPrimaryExpression(node);
}

void ToSource::visitPtrOperator(PtrOperatorAST *node)
{
  appendTokens(this, node->cv);
  appendToken(node->op);
  DefaultVisitor::visitPtrOperator(node);
}

// void ToSource::visitPtrToMember(PtrToMemberAST *)
// {
// 
// }
// 
// void ToSource::visitReturnStatement(ReturnStatementAST *node)
// {
// }

void ToSource::visitSimpleDeclaration(SimpleDeclarationAST *node)
{
  appendTokens(this, node->storage_specifiers);
  appendTokens(this, node->function_specifiers);
  DefaultVisitor::visitSimpleDeclaration(node);
}

void ToSource::visitSimpleTypeSpecifier(SimpleTypeSpecifierAST *node)
{
  appendTokens(this, node->cv);
  appendTokens(this, node->integrals);
  appendToken(node->type_of);
  DefaultVisitor::visitSimpleTypeSpecifier(node);
}

void ToSource::visitSizeofExpression(SizeofExpressionAST *node)
{
  appendToken(node->sizeof_token);
  DefaultVisitor::visitSizeofExpression(node);
}

void ToSource::visitStringLiteral(StringLiteralAST *node)
{
  appendTokens(this, node->literals);
  DefaultVisitor::visitStringLiteral(node);
}

// void ToSource::visitSubscriptExpression(SubscriptExpressionAST *node)
// {
// }
// 
// void ToSource::visitSwitchStatement(SwitchStatementAST *node)
// {
// }
// 
// void ToSource::visitTemplateArgument(TemplateArgumentAST *node)
// {
// }

void ToSource::visitTemplateDeclaration(TemplateDeclarationAST *node)
{
  appendToken(node->exported);
  DefaultVisitor::visitTemplateDeclaration(node);
}

// void ToSource::visitTemplateParameter(TemplateParameterAST *node)
// {
// }

void ToSource::visitThrowExpression(ThrowExpressionAST *node)
{
  appendToken(node->throw_token);
  DefaultVisitor::visitThrowExpression(node);
}

// void ToSource::visitTranslationUnit(TranslationUnitAST *node)
// {
// }
// 
// void ToSource::visitTryBlockStatement(TryBlockStatementAST *node)
// {
// }
// 
// void ToSource::visitTypeId(TypeIdAST *node)
// {
// }

void ToSource::visitTypeIdentification(TypeIdentificationAST *node)
{
  appendToken(node->typename_token);
  DefaultVisitor::visitTypeIdentification(node);
}

void ToSource::visitTypeParameter(TypeParameterAST *node)
{
  appendToken(node->type);
  DefaultVisitor::visitTypeParameter(node);
}

// void ToSource::visitTypedef(TypedefAST *node)
// {
// }

void ToSource::visitUnaryExpression(UnaryExpressionAST *node)
{
  appendToken(node->op);
  DefaultVisitor::visitUnaryExpression(node);
}

void ToSource::visitUnqualifiedName(UnqualifiedNameAST *node)
{
  appendToken(node->tilde);
  appendToken(node->id);
  DefaultVisitor::visitUnqualifiedName(node);
}

void ToSource::visitUsing(UsingAST *node)
{
  appendToken(node->type_name);
  DefaultVisitor::visitUsing(node);
}

// void ToSource::visitUsingDirective(UsingDirectiveAST *node)
// {
// }
// 
// void ToSource::visitWhileStatement(WhileStatementAST *node)
// {
// }

void ToSource::visitWinDeclSpec(WinDeclSpecAST *node)
{
  appendToken(node->specifier);
  appendToken(node->modifier);
  DefaultVisitor::visitWinDeclSpec(node);
}

// void ToSource::visitStatementName(StatementNameAST *)
// {
// }
