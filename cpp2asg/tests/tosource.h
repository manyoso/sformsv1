#ifndef TOSOURCE_H
#define TOSOURCE_H

#include "default_visitor.h"

#include <QUrl>
#include <QString>
#include <QList>

class ParseSession;
class DUContext;
class CppEditorIntegrator;

class ToSource: protected DefaultVisitor
{
public:
  ToSource(const QUrl &url);
  virtual ~ToSource();

  QString dump(AST *node, ParseSession* session);

  void appendToken(std::size_t token);

  void setDebug(bool d) { m_debug = d; }

protected:
  virtual void visit(AST *node);
  virtual void visitAccessSpecifier(AccessSpecifierAST *);
  virtual void visitAsmDefinition(AsmDefinitionAST *);
//   virtual void visitBaseClause(BaseClauseAST *);
  virtual void visitBaseSpecifier(BaseSpecifierAST *);
  virtual void visitBinaryExpression(BinaryExpressionAST *);
//   virtual void visitCastExpression(CastExpressionAST *);
  virtual void visitClassMemberAccess(ClassMemberAccessAST *);
  virtual void visitClassSpecifier(ClassSpecifierAST *);
//   virtual void visitCompoundStatement(CompoundStatementAST *);
//   virtual void visitCondition(ConditionAST *);
//   virtual void visitConditionalExpression(ConditionalExpressionAST *);
  virtual void visitCppCastExpression(CppCastExpressionAST *);
  virtual void visitCtorInitializer(CtorInitializerAST *);
//   virtual void visitDeclarationStatement(DeclarationStatementAST *);
  virtual void visitDeclarator(DeclaratorAST *);
  virtual void visitDeleteExpression(DeleteExpressionAST *);
//   virtual void visitDoStatement(DoStatementAST *);
  virtual void visitElaboratedTypeSpecifier(ElaboratedTypeSpecifierAST *);
//   virtual void visitEnumSpecifier(EnumSpecifierAST *);
  virtual void visitEnumerator(EnumeratorAST *);
  virtual void visitExceptionSpecification(ExceptionSpecificationAST *);
//   virtual void visitExpressionOrDeclarationStatement(ExpressionOrDeclarationStatementAST *);
//   virtual void visitExpressionStatement(ExpressionStatementAST *);
//   virtual void visitForStatement(ForStatementAST *);
  virtual void visitFunctionCall(FunctionCallAST *);
  virtual void visitFunctionDefinition(FunctionDefinitionAST *);
//   virtual void visitIfStatement(IfStatementAST *);
  virtual void visitIncrDecrExpression(IncrDecrExpressionAST *);
//   virtual void visitInitDeclarator(InitDeclaratorAST *);
//   virtual void visitInitializer(InitializerAST *);
//   virtual void visitInitializerClause(InitializerClauseAST *);
//   virtual void visitLabeledStatement(LabeledStatementAST *);
//   virtual void visitLinkageBody(LinkageBodyAST *);
  virtual void visitLinkageSpecification(LinkageSpecificationAST *);
  virtual void visitMemInitializer(MemInitializerAST *);
//   virtual void visitName(NameAST *);
  virtual void visitNamespace(NamespaceAST *);
  virtual void visitNamespaceAliasDefinition(NamespaceAliasDefinitionAST *);
//   virtual void visitNewDeclarator(NewDeclaratorAST *);
  virtual void visitNewExpression(NewExpressionAST *);
  virtual void visitNewInitializer(NewInitializerAST *);
//   virtual void visitNewTypeId(NewTypeIdAST *);
  virtual void visitOperator(OperatorAST *);
//   virtual void visitOperatorFunctionId(OperatorFunctionIdAST *);
//   virtual void visitParameterDeclaration(ParameterDeclarationAST *);
  virtual void visitParameterDeclarationClause(ParameterDeclarationClauseAST *);
  virtual void visitPostfixExpression(PostfixExpressionAST *);
  virtual void visitPrimaryExpression(PrimaryExpressionAST *);
  virtual void visitPtrOperator(PtrOperatorAST *);
//   virtual void visitPtrToMember(PtrToMemberAST *);
//   virtual void visitReturnStatement(ReturnStatementAST *);
  virtual void visitSimpleDeclaration(SimpleDeclarationAST *);
  virtual void visitSimpleTypeSpecifier(SimpleTypeSpecifierAST *);
  virtual void visitSizeofExpression(SizeofExpressionAST *);
  virtual void visitStringLiteral(StringLiteralAST *);
//   virtual void visitSubscriptExpression(SubscriptExpressionAST *);
//   virtual void visitSwitchStatement(SwitchStatementAST *);
//   virtual void visitTemplateArgument(TemplateArgumentAST *);
  virtual void visitTemplateDeclaration(TemplateDeclarationAST *);
//   virtual void visitTemplateParameter(TemplateParameterAST *);
  virtual void visitThrowExpression(ThrowExpressionAST *);
//   virtual void visitTranslationUnit(TranslationUnitAST *);
//   virtual void visitTryBlockStatement(TryBlockStatementAST *);
//   virtual void visitTypeId(TypeIdAST *);
  virtual void visitTypeIdentification(TypeIdentificationAST *);
  virtual void visitTypeParameter(TypeParameterAST *);
//   virtual void visitTypedef(TypedefAST *);
  virtual void visitUnaryExpression(UnaryExpressionAST *);
  virtual void visitUnqualifiedName(UnqualifiedNameAST *);
  virtual void visitUsing(UsingAST *);
//   virtual void visitUsingDirective(UsingDirectiveAST *);
//   virtual void visitWhileStatement(WhileStatementAST *);
  virtual void visitWinDeclSpec(WinDeclSpecAST *);
//   virtual void visitStatementName(StatementNameAST *);

private:
  QUrl m_url;
  bool m_debug;
  ParseSession* m_session;
  CppEditorIntegrator* m_editor;
  QList<QString*> m_strings;
};

template <class _Tp>
  void appendTokens(ToSource *s, const ListNode<_Tp> *nodes)
  {
    if (!nodes)
      return;

    const ListNode<_Tp>
      *it = nodes->toFront(),
      *end = it;

    do
      {
        s->appendToken(it->element);
        it = it->next;
      }
    while (it != end);
  }

#endif
