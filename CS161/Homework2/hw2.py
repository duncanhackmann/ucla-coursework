# Trees are nested tuples; a leaf is any non-tuple value.
# State for the river-crossing problem: (human, bunyip, dragon, paladin)
# where True = far side, False = near side. All must reach the far side.
# The human can only transport one passenger per crossing.
# Unsafe states: bunyip+dragon or bunyip+paladin alone without the human.

def BFS(TREE):
    """Return the leaves of TREE in breadth-first order."""
    if not isinstance(TREE, tuple):
        return (TREE,)

    queue = [TREE]
    leaves = []

    while queue:
        node = queue.pop(0)

        if isinstance(node, tuple):
            for child in node:
                queue.append(child)
        else:
            leaves.append(node)

    return tuple(leaves)

def DFS(TREE):
    """Return the leaves of TREE in depth-first (left-to-right) order."""
    if not isinstance(TREE, tuple):
        return (TREE,)

    result = ()
    for child in TREE:
        if isinstance(child, tuple):
            result += DFS(child)
        else:
            result += (child,)
    return result

def DFID(TREE, D):
    """Return leaves visited by depth-first iterative deepening up to depth D."""
    def _dls(node, depth, limit, out):
        if not isinstance(node, tuple):
            out.append(node)
            return
        if depth == limit:
            return
        # Iterate children right-to-left so leftmost child is explored first
        for i in range(len(node) - 1, -1, -1):
            _dls(node[i], depth + 1, limit, out)

    result = []
    for limit in range(D + 1):
        _dls(TREE, 0, limit, result)
    return tuple(result)

def FINAL_STATE(S):
    """Return True if all four characters have crossed to the far side."""
    return S == (True, True, True, True)


def NEXT_STATE(S, A):
    """Return the state after action A, or [] if the move is impossible or unsafe.

    Actions: 'h' = human crosses alone, 'b'/'d'/'p' = human ferries that character.
    """
    h, b, d, p = S

    def unsafe(hh, bb, dd, pp):
        # Bunyip eats dragon or paladin if left alone with them
        return (bb == dd != hh) or (bb == pp != hh)

    if A == "h":
        nh, nb, nd, np = (not h), b, d, p
    elif A == "b":
        if h != b:
            return []
        nh, nb, nd, np = (not h), (not b), d, p
    elif A == "d":
        if h != d:
            return []
        nh, nb, nd, np = (not h), b, (not d), p
    elif A == "p":
        if h != p:
            return []
        nh, nb, nd, np = (not h), b, d, (not p)
    else:
        return []

    if unsafe(nh, nb, nd, np):
        return []

    return [(nh, nb, nd, np)]


def SUCC_FN(S):
    """Return all valid states reachable from S in one move."""
    out = []
    for a in ("h", "b", "d", "p"):
        nxt = NEXT_STATE(S, a)
        if nxt:
            out.append(nxt[0])
    return out


def ON_PATH(S, STATES):
    """Return True if state S has already been visited on the current path (cycle check)."""
    return S in STATES


def MULT_DFS(STATES, PATH):
    """Try DFS from each state in STATES; return the first solution found."""
    for s in STATES:
        sol = DFS_SOL(s, PATH)
        if sol != []:
            return sol
    return []


def DFS_SOL(S, PATH):
    """Return a solution path from S to the goal using DFS, or [] if none exists."""
    if FINAL_STATE(S):
        return PATH + [S]

    if ON_PATH(S, PATH):
        return []

    succ = SUCC_FN(S)
    if not succ:
        return []

    return MULT_DFS(succ, PATH + [S])


path = DFS_SOL((False, False, False, False), [])
print(path)