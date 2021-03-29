parallel_out = "output_parallel"
consistent_out = "output_consistent"

pos = 5


def comp(x, y):
    return "".join(str(x).split()[1:]) >= "".join(str(y).split()[1:])


def top_diff(list_par, list_con):
    list_par.sort()
    list_con.sort()
    for line_p, line_c in zip(list_par, list_con):
        if not line_c == line_p:
            if not str(line_c).split()[1:] == str(line_c).split()[1:]:
                print(line_c)
                print(line_p)
                return False

    return True


if __name__ == '__main__':
    with open(parallel_out, "r") as po, open(consistent_out, "r") as co:
        counter = 0
        list_p = []
        list_c = []
        for lp, lc, in zip(po, co):
            counter += 1
            list_p.append(lp[2:])
            list_c.append(lc[2:])
            if counter == pos + 1:
                if top_diff(list_p, list_c) == False:
                    # print('MISMATCH')
                    exit(-1)
                co.readline()
                po.readline()
                counter = 0
                list_p = []
                list_c = []

    exit(0)
