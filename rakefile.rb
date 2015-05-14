task :setup do
    sh 'gem install jekyll'
    sh 'sudo pacman -S nodejs'
end

task :serve do
    Dir.chdir('web'){sh 'jekyll serve'}
end
task :build do
    Dir.chdir('web'){sh 'jekyll build'}
end
task :publish => :build do
    Dir.chdir('web/_site') do
        sh 'scp -r * web-gfannes@ssh.linuxsystems.be:fannes.com/www/subsoil'
    end
end
