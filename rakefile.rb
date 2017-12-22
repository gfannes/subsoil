task :default do
    sh "rake -T"
end

desc "Install necessary packages for manjaro"
task :setup do
    sh 'gem install jekyll'
    sh 'sudo pacman -S nodejs'
end

desc "Serve the static pages with a local jekyll server"
task :serve do
    Dir.chdir('web'){sh 'jekyll serve'}
end

desc "Build the static pages"
task :build do
    Dir.chdir('web'){sh 'jekyll build'}
end

desc "Publish the static pages on the web"
task :publish => :build do
    Dir.chdir('web/_site') do
        sh 'scp -r * web-gfannes@ssh.linuxsystems.be:fannes.com/www/subsoil'
    end
end

desc "Start a new post"
task :post do
    puts "Page title:"
    title = STDIN.gets.chomp
    now = Time.now
    #Set time to 00:00:00, else, jekyll might not render it, maybe due to timezone or so
    date = now.strftime('%Y-%m-%d 00:00:00')
    filename = now.strftime('%Y-%m-%d-')+title.downcase.split(' ').join('-')+'.md'
    Dir.chdir('web/_posts') do
        File.open(filename, "w") do |fo|
            fo.puts('---')
            fo.puts('layout: post')
            fo.puts("title: #{title}")
            fo.puts("date: #{date}")
            fo.puts('categories: ')
            fo.puts('---')
        end
    end
end
