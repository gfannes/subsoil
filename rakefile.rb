task :setup do
    sh 'gem install jekyll'
    sh 'sudo pacman -S nodejs'
end

task :serve do
    Dir.chdir('web') do
        sh 'jekyll serve'
    end
end
