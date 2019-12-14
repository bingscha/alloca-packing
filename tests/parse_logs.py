info = open("parsed.txt", 'w')
logs = open("build/logs.txt", 'r')

file_information = {}

lines = logs.readlines()
i = 0
lines = [val.strip() for val in lines]
lines = [val for val in lines if val]

while i < len(lines):
    file_information[lines[i]] = []
    file_name = lines[i]
    file_information[file_name].append({'inst_count': 0,'loads': 0, 'stores': 0, 'allocas': 0, 'binary_and_unary': 0})
    file_information[file_name].append({'inst_count': 0,'loads': 0, 'stores': 0, 'allocas': 0, 'binary_and_unary': 0})
    i += 1
    while lines[i].split()[0] != 'Benchmarking':
        stats = lines[i].split()
        file_information[file_name][0]['inst_count'] += int(stats[1])
        file_information[file_name][0]['loads'] += int(stats[2])
        file_information[file_name][0]['stores'] += int(stats[3])
        file_information[file_name][0]['allocas'] += int(stats[4])
        file_information[file_name][0]['binary_and_unary'] += int(stats[5])
        i += 1

        stats = lines[i].split()
        file_information[file_name][1]['inst_count'] += int(stats[1])
        file_information[file_name][1]['loads'] += int(stats[2])
        file_information[file_name][1]['stores'] += int(stats[3])
        file_information[file_name][1]['allocas'] += int(stats[4])
        file_information[file_name][1]['binary_and_unary'] += int(stats[5])
        i += 1

    i += 2

    stats = lines[i].split()
    file_information[file_name][0]['time'] = float(stats[1].split('m')[1].rstrip('s'))

    i += 4

    stats = lines[i].split()
    file_information[file_name][1]['time'] = float(stats[1].split('m')[1].rstrip('s'))

    i += 3

info.write(str(file_information))
