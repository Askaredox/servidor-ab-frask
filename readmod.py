import json

class ReadMod:
    # path = "test-modulo"
    path = "/proc/modulo"
    @staticmethod
    def get_vals():
        ret = {}
        with open(ReadMod.path) as file:
            content = file.read()
            pcStatus = json.loads(content)

            used_mem = pcStatus['um_temp']
            total_mem = pcStatus['tm_temp']

            used_cpu = pcStatus['uc_temp']
            total_cpu = pcStatus['tc_temp']

            ret['cpu'] = ReadMod.division(used_cpu, total_cpu)
            ret['ram'] = ReadMod.division(used_mem, total_mem)
        return ret
    
    @staticmethod
    def division(a, b):
        try:
            ret = 100 * a / b
            return ret
        except ZeroDivisionError:
            return 0
