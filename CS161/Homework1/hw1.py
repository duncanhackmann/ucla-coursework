# Trees are represented as nested tuples. A leaf is any non-tuple value.
# Binary tree nodes have the form (left, value, right).

def PAD(n):
    """Return the nth value of the Padovan sequence (PAD(0)=PAD(1)=PAD(2)=1)."""
    if n <= 2:
        return 1
    return PAD(n - 2) + PAD(n - 3)

def SUMS(n):
    """Return the number of addition operations needed to compute PAD(n) recursively."""
    if n <= 2:
        return 0
    return 1 + SUMS(n - 2) + SUMS(n - 3)

def ANON(TREE):
    """Return a copy of TREE with every leaf replaced by '?', preserving structure."""
    if isinstance(TREE, tuple):
        return tuple(ANON(sub) for sub in TREE)
    return '?'

def TREE_HEIGHT(TREE):
    """Return the height of TREE (0 for a leaf, 1 + max child height otherwise)."""
    if not isinstance(TREE, tuple):
        return 0
    if not TREE:
        return 0
    return 1 + max(TREE_HEIGHT(sub) for sub in TREE)


def TREE_ORDER(TREE):
    """Return a post-order traversal of a binary tree: left subtree, right subtree, root."""
    if not isinstance(TREE, tuple):
        return (TREE,)

    L, m, R = TREE
    return TREE_ORDER(L) + TREE_ORDER(R) + (m,)



