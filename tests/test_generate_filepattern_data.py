# -*- coding: utf-8 -*-
import math
import os


directory = 'test_data'
root_directory = os.path.dirname(os.path.realpath(__file__))
path = os.path.join(root_directory, directory)


def generate_data():
    MAX = 100
    length = 0

    data_path = path + '/data' + str(MAX)

    try:
        os.mkdir(path)
        print('Data directory created at ' + path)
    except FileExistsError:
        print("Data directory already exists")

    try:
        os.mkdir(data_path)
        print('Data directory created at ' + data_path)
    except FileExistsError:
        print("Data directory already exists")

    limit = int(math.ceil(math.sqrt(MAX)))

    infile = open(path + "/data" + str(MAX) + ".txt", "w+")
    for i in range(0, limit):

        str_i = str(i).zfill(3)

        for j in range(0, limit):

            str_j = str(j).zfill(3)
            length += 1
            data_name = 'img_r{}_c{}.tif'.format(str_i, str_j)
            infile.write(data_name + '\n')
            f = open(data_path + '/' + data_name, 'w+')
            f.close()
    infile.close()
    print(str(length) + " files generated.")


def generate_channel_data():

    MAX = 3

    directory = 'test_data'
    root_directory = os.path.dirname(os.path.realpath(__file__))
    path = os.path.join(root_directory, directory)
    data_path = path + '/data'
    recursive_path = path + '/recursive_data'

    try:
        os.mkdir(path)
        os.mkdir(data_path)
        os.mkdir(recursive_path)
        print('Data directory created at ' + path)
    except FileExistsError:
        print("Data directory already exists")

    try:
        os.mkdir(data_path)
        print('Data directory created at ' + path)
    except FileExistsError:
        print("Data directory already exists")

    try:
        os.mkdir(recursive_path)
        print('Data directory created at ' + path)
    except FileExistsError:
        print("Data directory already exists")

    channels = ['DAPI', 'TXREAD', 'GFP']

    for channel in channels:
        current_path = recursive_path + '/' + channel
        try:
            os.mkdir(current_path)
        except FileExistsError:
            print("Channel directory already exists")

        for i in range(0, MAX):

            str_i = str(i).zfill(3)

            for j in range(0, MAX):

                str_j = str(j).zfill(3)

                data_name = 'img_r{}_c{}_{}.tif'.format(str_i, str_j, channel)
                f = open(data_path + '/' + data_name, 'w+')
                f.close()

                recursive_name = 'img_r{}_c{}.tif'.format(str_i, str_j)
                f = open(current_path + '/' + recursive_name, 'w+')
                f.close()

    print("Files generated.")


def generate_channel_data_sc():

    MAX = 3

    directory = 'test_data'
    root_directory = os.path.dirname(os.path.realpath(__file__))
    path = os.path.join(root_directory, directory)
    data_path = path + '/data'
    recursive_path = path + '/recursive_data_sc'

    try:
        os.mkdir(path)
        os.mkdir(data_path)
        os.mkdir(recursive_path)
        print('Data directory created at ' + path)
    except FileExistsError:
        print("Data directory already exists")

    try:
        os.mkdir(data_path)
        print('Data directory created at ' + path)
    except FileExistsError:
        print("Data directory already exists")

    try:
        os.mkdir(recursive_path)
        print('Data directory created at ' + path)
    except FileExistsError:
        print("Data directory already exists")

    channels = ['DAPI_TEST', 'TXREAD_TEST', 'GFP_TEST']

    for channel in channels:
        current_path = recursive_path + '/' + channel
        try:
            os.mkdir(current_path)
        except FileExistsError:
            print("Channel directory already exists")

        for i in range(0, MAX):

            str_i = str(i).zfill(3)

            for j in range(0, MAX):

                str_j = str(j).zfill(3)
                
                recursive_name = 'img_r{}_c{}.tif'.format(str_i, str_j)
                f = open(current_path + '/' + recursive_name, 'w+')
                f.close()

    print("Files generated.")


def generate_channel_data_spaces():

    MAX = 3

    directory = 'test_data'
    root_directory = os.path.dirname(os.path.realpath(__file__))
    path = os.path.join(root_directory, directory)
    data_path = path + '/data'
    recursive_path = path + '/recursive_data_spaces'

    try:
        os.mkdir(path)
        os.mkdir(data_path)
        os.mkdir(recursive_path)
        print('Data directory created at ' + path)
    except FileExistsError:
        print("Data directory already exists")

    try:
        os.mkdir(data_path)
        print('Data directory created at ' + path)
    except FileExistsError:
        print("Data directory already exists")

    try:
        os.mkdir(recursive_path)
        print('Data directory created at ' + path)
    except FileExistsError:
        print("Data directory already exists")

    channels = ['DAPI TEST', 'TXREAD TEST', 'GFP TEST']

    for channel in channels:
        current_path = recursive_path + '/' + channel
        try:
            os.mkdir(current_path)
        except FileExistsError:
            print("Channel directory already exists")

        for i in range(0, MAX):

            str_i = str(i).zfill(3)

            for j in range(0, MAX):

                str_j = str(j).zfill(3)

                recursive_name = 'img_r{}_c{}.tif'.format(str_i, str_j)
                f = open(current_path + '/' + recursive_name, 'w+')
                f.close()

    print("Files generated.")


def generate_sorted_data():
    MAX = 30
    length = 0

    directory = 'test_data'
    root_directory = os.path.dirname(os.path.realpath(__file__))
    path = os.path.join(root_directory, directory)
    data_path = path + '/sorted_data'

    try:
        os.mkdir(path)
        print('Data directory created at ' + path)
    except FileExistsError:
        print("Data directory already exists")

    try:
        os.mkdir(data_path)
        print('Data directory created at ' + data_path)
    except FileExistsError:
        print("Data directory already exists")

    for i in range(0, MAX):

        data_name = '{}.tif'.format(str(i))
        f = open(data_path + '/' + data_name, 'w+')
        f.close()

    print(str(length) + " files generated.")


def generate_text_data():
    output_file = path + '/data100.txt'
    print(output_file)
    with open(output_file, "w") as file:
        for r in range(10):
            for c in range(10):
                filename = f"img_r{r:03}_c{c:03}.tif"
                file.write(filename + "\n")


def generate_bracket_data():
    directory = 'test_data'
    root_directory = os.path.dirname(os.path.realpath(__file__))
    path = os.path.join(root_directory, directory)
    data_path = path + '/bracket_data'

    try:
        os.mkdir(path)
        print('Data directory created at ' + path)
    except FileExistsError:
        print("Data directory already exists")

    try:
        os.mkdir(data_path)
        print('Data directory created at ' + data_path)
    except FileExistsError:
        print("Data directory already exists")

    for i in range(0, MAX):
        data1 = 'x(0-31)_y(01-48)_c0.ome.tif'
        data2 = 'x(0-31)_y(01-48)_c1.ome.tif'
        f1 = open(data_path + '/' + data1, 'w+')
        f1.close()
        f2 = open(data_path + '/' + data2, 'w+')
        f2.close()


def generate_recursive_no_capture_data():
    base_data_dir_name = 'recursive_no_capture_data'
    subdir1_name = 'subdir1'
    subdir2_name = 'subdir2'
    num_files_subdir1 = 5
    num_files_subdir2 = 7
    file_extension = '.tmp'

    recursive_base_path = os.path.join(path, base_data_dir_name)
    subdir1_path = os.path.join(recursive_base_path, subdir1_name)
    subdir2_path = os.path.join(recursive_base_path, subdir2_name)

    try:
        os.makedirs(recursive_base_path, exist_ok=True)
        os.makedirs(subdir1_path, exist_ok=True)
        os.makedirs(subdir2_path, exist_ok=True)
        print(f"Created directory structure at {recursive_base_path}")
    except OSError as e:
        print(f"Error creating directories: {e}")
        return  # Stop if directories can't be created

    # Create files in subdir1
    for i in range(num_files_subdir1):
        file_name = f"file{i+1}{file_extension}"
        try:
            with open(os.path.join(subdir1_path, file_name), 'w'):
                pass  # Create empty file
        except IOError as e:
            print(f"Error creating file {os.path.join(subdir1_path, file_name)}: {e}")

    # Create files in subdir2
    for i in range(num_files_subdir2):
        file_name = f"item{chr(ord('A')+i)}{file_extension}"  # e.g., itemA.tmp, itemB.tmp
        try:
            with open(os.path.join(subdir2_path, file_name), 'w'):
                pass  # Create empty file
        except IOError as e:
            print(f"Error creating file {os.path.join(subdir2_path, file_name)}: {e}")

    total_files = num_files_subdir1 + num_files_subdir2
    print(f"{total_files} files generated in {recursive_base_path}")


if __name__ == '__main__':
    generate_data()
    generate_channel_data()
    generate_sorted_data()
    generate_text_data()
    generate_bracket_data()
    generate_channel_data_sc()
    generate_channel_data_spaces()
    generate_recursive_no_capture_data()


MAX = 3

directory = 'test_data'
root_directory = os.path.dirname(os.path.realpath(__file__))
path = os.path.join(root_directory, directory)
data_path = path + '/data'
recursive_path = path + '/recursive_data'
sorted_data = path + '/sorted'

sp_data = path + "/sp_data.txt"

try:
    os.mkdir(path)
    os.mkdir(data_path)
    os.mkdir(recursive_path)
    os.mkdir(sorted_data)
    print('Data directory created at ' + path)
except FileExistsError:
    print("Data directory already exists")

channels = ['DAPI', 'TXREAD', 'GFP']

infile = open(sp_data, 'w')

for channel in channels:
    current_path = recursive_path + '/' + channel
    try:
        os.mkdir(current_path)
    except FileExistsError:
        print("Data directory already exists")

    for i in range(0, MAX):

        str_i = str(i).zfill(3)

        for j in range(0, MAX):

            str_j = str(j).zfill(3)

            data_name = 'img_r{}_c{}_{}.tif'.format(str_i, str_j, channel)
            infile.write(data_name + '\n')
            f = open(data_path + '/' + data_name, 'w+')
            f.close()

            recursive_name = 'img_r{}_c{}.tif'.format(str_i, str_j)
            f = open(current_path + '/' + recursive_name, 'w+')
            f.close()
output_file = path + '/data100.txt'
print(output_file)
with open(output_file, "w") as file:
    for r in range(10):
        for c in range(10):
            filename = f"img_r{r:03}_c{c:03}.tif"
            file.write(filename + "\n")

infile.close()
print("Files generated.")
