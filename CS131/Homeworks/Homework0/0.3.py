poop = [1, 3, 5, 7, 9]
jerry = []

fart = poop[1:]
print(fart)

seven = 7

emmett = [seven] + poop
print(emmett)

print(len(emmett))

def find_biggest(emmett):
    if len(emmett) == 1:
        return emmett[0]
    rest = find_biggest(emmett[1:])
    if rest > emmett[0]:
        return rest
    return emmett[0]

