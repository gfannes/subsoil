import os.path
import os

class Filepath:
    __slots__ = 'abs_dir', 'rel_dir', 'name'
    def __init__(self, fp=None):
        for name in self.__slots__:
            setattr(self, name, None)
        if fp is not None:
            self.name = os.path.basename(fp)
            dir = os.path.dirname(fp)
            if os.path.isabs(fp):
                self.abs_dir = dir
            else:
                self.abs_dir = os.getcwd()
                self.rel_dir = dir
    def path(self):
        ary = [getattr(self,name) for name in self.__slots__ if getattr(self,name) is not None]
        return os.path.join(*ary)
    def __str__(self):
        return f'[{type(self).__name__}]'+''.join([f'({name}:{getattr(self,name)})' for name in self.__slots__])
