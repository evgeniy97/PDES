import pandas as pd
import sys as sys # to get argument from command line

# No error calculate in this version

number_of_simulation = 1000 
number_of_timesteps = 100000 # Unused
def main(path):
    """""
    path: path to directory
    """""
    # Initiliaze colum names
    file_path = path + '/test{}.txt'
    names =['step','min','max','average','width','utilization']
    data = pd.read_table(file_path.format(0),sep=' ',names=names,header=None,index_col='step')
    for i in range(1, number_of_simulation):
        data += pd.read_table(file_path.format(i),sep=' ',names=names,header=None,index_col='step')

    data /= number_of_simulation

    # Save data
    # There may be a sence to save output in new directory
    out_file_path = path + '.txt'

    data.to_csv(out_file_path, header=None, sep=' ', mode='w')
    # read txt file as data = pd.read_table(FILE_PATH,sep=' ',names=names,header=None,index_col='step')



if __name__ == "__main__":
    main(sys.argv[1]) # sys.argv[0] is a name of the script