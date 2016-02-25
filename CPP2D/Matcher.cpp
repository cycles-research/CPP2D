#include "Matcher.h"
#include "MatchContainer.h"

using namespace clang::ast_matchers;

MatchFinder getMatcher(MatchContainer& receiver)
{
	StatementMatcher foreachDeclInitMatcher =
		cxxForRangeStmt(hasLoopVariable(varDecl(anything()).bind("forrange_loopvar")));

	StatementMatcher foreachDeclAutoMatcher =
		cxxForRangeStmt(hasLoopVariable(varDecl(
			anyOf(
				hasType(lValueReferenceType(pointee(autoType().bind("forrange_loopvar_auto")))),
				hasType(autoType().bind("forrange_loopvar_auto"))
				))));

	MatchFinder finder;
	finder.addMatcher(foreachDeclInitMatcher, &receiver);
	finder.addMatcher(foreachDeclAutoMatcher, &receiver);
	return finder;
}