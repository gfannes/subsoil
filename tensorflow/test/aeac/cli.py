import aeac.pod

import argparse
import os
import glob

class Options:
    __slots__ = 'mode', 'inputs', 'output', 'block_size'
    def __init__(self):
        self.mode = None
        self.inputs = []
        self.output = None
        self.block_size = 32
    def __str__(self):
        lines = []
        lines.append(f'[Options](mode:{self.mode})(output:{self.output})(block_size:{self.block_size}){{')
        for input in self.inputs:
            lines.append(f'  {input}')
        lines.append(f'}}')
        return '\n'.join(lines)

def get_options():
    options = Options()

    parser = argparse.ArgumentParser()
    parser.add_argument('-m', '--mode', dest='mode', action='store', choices={'create','encode','decode','encdec','train'}, required=True)
    parser.add_argument('-i', '--input', dest='input', action='store', help='Input file or folder')
    parser.add_argument('-o', '--output', dest='output', action='store', help='Output file or folder')
    parser.add_argument('-b', '--block_size', dest='block_size', action='store', default=options.block_size, help=f'Block size, default {options.block_size}')
    raw_args = parser.parse_args()
    if 1*'Print raw CLI arguments':
        print( f'CLI arguments before processing: {raw_args}' )
    
    for name in ['mode','block_size','output']:
        setattr(options, name, getattr(raw_args, name))
    if 'process inputs':
        extension = {'create':'wav'}.get(options.mode, None)
        if os.path.isdir(raw_args.input):
            if extension is None:
                raise SystemExit( f'No globbing supported in mode "{raw_args.mode}" for folder "{raw_args.input}".' )
            for fp in glob.iglob(os.path.join(raw_args.input, f'**/*.{extension}'), recursive=True):
                options.inputs.append(aeac.pod.Filepath(fp))
        else:
            options.inputs.append(aeac.pod.Filepath(raw_args.input))

    return options


