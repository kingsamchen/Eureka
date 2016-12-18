
def krange(begin, end):
    value = begin
    while value < end:
        yield value
        value += 1
    return


def main():
    for i in krange(0, 10):
        print(i)


if __name__ == '__main__':
    main()
