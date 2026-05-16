from collections import UserString

# In C:
# typedef struct {
#     char *data;
#     size_t len;
# } StringView;

class StringView(UserString):
    def __init__(self, seq):
        super().__init__(seq)

        self.ptr = 0
        self.len = len(seq)

    def chop_l(self, n, /):
        # In C:
        # s.str += n;
        # s.len -= n;

        self.ptr += n
        self.len -= n

    def chop_r(self, n, /):
        # In C:
        # s.len -= n;

        self.len -= n

    def slice(self):
        return self.data[self.ptr:self.ptr+self.len]

    def reset(self):
        self.ptr = 0
        self.len = len(self.data)

def main():
    print(
        "===============================================================================\n"
        "> UserString is a wrapper around 'str' to create string-like subclasses.\n"
        "> It's not that useful as one can already make a subclass of 'str', but it\n"
        "> already does some of the work.\n"
        ">\n"
        "> This example implements a StringView class, which is a common C struct that\n"
        "> allows you to manipulate a string by its length, not by where '\\0' is.\n"
        "> In C, this is useful to handle strings without modifying the original value.\n"
        ">\n"
        "> Docs: https://docs.python.org/3.14/library/collections.html#collections.UserString \n"
        "> Translated from C: https://github.com/kerudev/parsec/blob/76657b8/examples/02-strings/main.c \n"
        "===============================================================================\n"
    )

    l = 10
    r = 18

    data = "The quick brown fox jumps over the lazy dog"
    view = StringView(data)

    print("Printing the whole string")

    # In C:
    # printf("- char*       : %s (length = %zu)\n", data, strlen(data));
    # printf("- ParsecString: %s (length = %zu)\n", view.str, view.len);
    print("- str       : %s (length = %d)" % (data, len(data)))
    print("- StringView: %s (length = %d)" % (view.data, view.len))

    # In C:
    # char *s = strdup(str);
    # s += l;                 // chop left
    # s[strlen(s) - r] = 0;   // chop right

    s = data[:]
    s = s[l:]         # quick brown fox jumps over the lazy dog
    s = s[:len(s)-r]  # quick brown fox jumps

    # In C:
    # view.str += l;          // chop left
    # view.len -= l;          // chop left
    # view.len -= r;          // chop right

    view.chop_l(l)    # quick brown fox jumps over the lazy dog
    view.chop_r(r)    # quick brown fox jumps

    # In C:
    # printf("- char*       : %s (length = %zu)\n", s, strlen(s));
    # printf("- ParsecString: %s (length = %zu)\n", view.str, view.len);
    print()
    print("Printing the whole string (after chopping)")
    print("- str       : %s (length = %d)" % (s, len(s)))
    print("- StringView: %s (length = %d)" % (view.data, view.len))

    print()
    print(
        "===============================================================================\n"
        "> Noticed how StringView printed more than its length? This is because the\n"
        "> string inside it is being printed until '\\0', not by length.\n"
        "==============================================================================="
    )

    # In C:
    # printf("- char*       : %.*s (length = %zu)\n", (int)strlen(s), s, strlen(s));
    # printf("- ParsecString: %.*s (length = %zu)\n", (int)view.len, view.str, view.len);

    print()
    print("Printing by length")
    print("- str       : %s (length = %d)" % (s, len(s)))
    print("- StringView: %s (length = %d)" % (view.slice(), view.len))

    # In C:
    # s -= l;
    # s[strlen(s) + r] = ' ';

    s = ' ' * l + s
    s = s + ' ' * r
    view.reset()       # The quick brown fox jumps over the lazy dog

    print()
    print("Resetting the string")
    print("- str       : %s (length = %d)" % (s, len(s)))
    print("- StringView: %s (length = %d)" % (view.slice(), view.len))

    print()
    print(
        "===============================================================================\n"
        "> This is why StringView is useful. The original string gets destroyed as it\n"
        "> gets chopped and there is no way to recover data.\n"
        "==============================================================================="
    )
