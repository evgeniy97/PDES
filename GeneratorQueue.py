import sys as sys # to get argument from command line
import config

# We use sbatch to control resources
command_to_run_simulation = 'sbatch -n 10 -t 14400 --wrap "mpirun -n 10 OSW+.o {N} {M} {r} {q} {p}"' # use string.format method
command_to_run__first_statistics = 'sbatch -n 1 -t 14400 --wrap "mpirun -n 1 st.o {} "'

def main(mode = 'simulation'):
    # mode: 'simulation' - run simulation
    #       'statistic' - run statistic
    # generate dictionary
    command = ''
    if mode == 'simulation':
        command = command_to_run_simulation

    with open('script','w') as file:
        for N in config.N_list:
            for M in config.M_list:
                for r in config.r_list:
                    for q in config.q_list:
                        for p in config.p_list:
                            file.write(
                                command.format(**{'N': N, 'M': M, 'r': r, 'q': q, 'p': p}) + '\n'
                            )



    return 0



if __name__ == "__main__":
    if len(sys.argv) > 1:
        main(sys.argv[1])
    else:
        main()