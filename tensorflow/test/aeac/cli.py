import argparse
import os
import glob

def parse_cli_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input', dest='inputs', metavar='INPUT', action='append', default=[])
    parser.add_argument('-o', '--output', dest='output', action='store')
    parser.add_argument('-b', '--blocksize', dest='blocksize', action='store', default=64)
    parser.add_argument('-m', '--mode', dest='mode', action='store', choices={'encode','decode','encdec','train'}, required=True)
    cli_args = parser.parse_args()
    print( f'CLI arguments before processing: {cli_args}' )

    if 'process inputs':
        inputs = []
        extension = {'train':'wav'}.get(cli_args.mode, None)
        for fp in cli_args.inputs:
            if os.path.isdir(fp):
                if extension is None:
                    raise SystemExit( f'No globbing supported in mode "{cli_args.mode}" for folder "{fp}".' )
                inputs.extend(glob.iglob(os.path.join(fp, f'**/*.{extension}'), recursive=True))
            else:
                inputs.append(fp)
        cli_args.inputs = inputs
    print( f'CLI arguments after processing: {cli_args}' )
    return cli_args


