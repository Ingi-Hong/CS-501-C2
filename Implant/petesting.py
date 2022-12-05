import pefile
import json

path = "tasks.exe"
pe = pefile.PE(path)

pe_import_dict = {}
# walk the Imports
for entry in pe.DIRECTORY_ENTRY_IMPORT:
    dll_name = entry.dll.decode('utf-8')
    pe_import_dict[dll_name] = []
    for func in entry.imports:
        pe_import_dict[dll_name] += [func.name.decode('utf-8')]

		
#print(json.dumps(pe_import_dict, indent=4, sort_keys=True))
print(pe_import_dict['KERNEL32.dll'])