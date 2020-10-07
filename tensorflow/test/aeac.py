#!/usr/bin/python

import aeac

args = aeac.parse_cli_args()

if args.mode=='train':
    data = aeac.create_data(args.inputs, args.blocksize)
    aeac.train_model(data)

if args.output:
    aeac.save_wav(args.output, data, 48000)
