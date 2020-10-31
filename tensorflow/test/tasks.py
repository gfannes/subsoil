from invoke import task
import subprocess

@task
def create_content(c):
    commands = [
            'sox -n audio/impulse.wav synth 1s square pad 0s 63s',
            ]
    for command in commands:
        print(command)
        subprocess.run(command.split(' '))

@task
def test(c):
    scenarios = [
            # {'args': '-h'},
            # {'args': '-i audio/WelcomeToAuro3D.wav', 'rc':2},
            # {'args': '-m create -i audio/impulse.wav -o data'},
            # {'args': '-m create -i audio -o data.wav'},
            {'args': '-m test -i audio/WelcomeToAuro3D.wav -o data.wav'},
            # {'args': '-m create -i audio -o data'},
            ]
    for scenario in scenarios:
        print(f'\n====================================\nRunning scenario {scenario["args"]}')
        cmd = './aeac.py ' + scenario['args']
        cp = subprocess.run(cmd.split(' '))
        rc_act = cp.returncode
        rc_exp = scenario.get('rc', 0)
        assert rc_act==rc_exp, f'Expected RC {rc_exp}, got {rc_act}'
