#ifndef __LANG_AST_ASTDEF_HH__
#define __LANG_AST_ASTDEF_HH__

#include <memory>

namespace langI
{
struct INode;
class ScopeNode;
class FuncDeclNode;
class VarDeclNode;
class DeclNode;
struct IStoreable;

using pSNode = std::shared_ptr<ScopeNode>;
using pwSNode = std::weak_ptr<ScopeNode>;
using pINode = std::shared_ptr<INode>;
using pIStoreable = std::shared_ptr<IStoreable>;
using pDNode = std::shared_ptr<DeclNode>;
} // namespace langI

#endif /* __LANG_AST_ASTDEF_HH__ */
