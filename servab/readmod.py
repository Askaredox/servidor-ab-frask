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

            used_mem = pcStatus['u_mem']
            total_mem = pcStatus['t_mem']

            used_cpu = pcStatus['u_cpu']
            total_cpu = pcStatus['t_cpu']

            ret['cpu'] = ReadMod.division(used_cpu, total_cpu)
            ret['ram'] = ReadMod.division(used_mem, total_mem)
        return ret
    
    @staticmethod
    def division(a, b):
        try:
            ret = 100 * a / b
            return int(ret)
        except ZeroDivisionError:
            return 0
