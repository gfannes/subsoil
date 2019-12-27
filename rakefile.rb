task :default do
    sh "rake -T"
end

desc "Clean"
task :clean do
    rm FileList.new("*.a")
    rm FileList.new("*.resp")
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

def gubg_arduino()
    require("arduino")
    "#{ENV["gubg"]}/gubg.arduino"
end

def cooker()
    require("gubg/shared")
    require("gubg/build/Cooker")
    c = GUBG::Build::Cooker.new
    case GUBG::os
    when :windows then c.option("c++.std", 14)
    else c.option("c++.std", 17) end
    c
end

namespace :wt6wd do
    desc "Build for arduino"
    task :main, :arch do |t,args|
        uri, arch = "wt6wd/main", args[:arch]||"mega"
        output_dir = "wt6wd/build/main/#{arch}"
        cooker()
            .recipes_fn(gubg_arduino, "recipes.chai")
            .recipes_fn("wt6wd/recipes.chai")
            .toolchain("gcc", "#{gubg_arduino}/cook/avr.chai")
            .option(arch)
            .output(output_dir)
            .generate(:ninja, uri)
            .ninja()
        Arduino.program("#{output_dir}/#{uri.gsub("/",".")}", arch: arch)
    end
end

namespace :fri3d do
    desc "Neural network test app"
    task :nn do
        cooker().generate(:ninja, "fri3d/nn").ninja().run()
    end
    desc "MLP test app"
    task :mlp do
        cooker().option("debug").option("ctng.tuple_", "x86_64-unknown-linux-gnu").generate(:ninja, "fri3d/mlp").ninja().run()
    end
    desc "Create MLPs"
    task :create_mlps do
        cooker().generate(:ninja, "fri3d/create_mlps").ninja().run()
    end
    [:boat, :prop].each do |name|
        desc "Build and flash the #{name} arduino app"
        task name, :arch do |t,args|
            arch = args[:arch]
            archs = %w[uno mega lilypad]
            raise("Invalid arch \"#{arch}\", should be one of #{archs*' '}") unless archs.include?(arch)

            output_dir = "fri3d/2018/build/#{arch}"
            sh "cook -t gcc -t #{gubg_arduino}/cook/avr.chai -T #{arch} -g ninja -g naft -o #{output_dir} -f #{gubg_arduino} -f ./ /fri3d/#{name}"
            # sh "ninja -f #{output_dir}/build.ninja -t clean"
            sh "ninja -f #{output_dir}/build.ninja -v"

            Arduino.program("#{output_dir}/fri3d.#{name}", arch: arch)
        end
    end
end

desc "PoC"
task :poc, [:filename, :arch] do |t,args|
    filename = args[:filename]
    ENV["subsoil_poc_filename"] = filename
    uri, arch = "poc/exe", args[:arch]||"mega"
    output_dir = "poc/build/#{filename}/#{arch}"
    cooker()
        .recipes_fn(gubg_arduino, "recipes.chai")
        .toolchain("gcc", "#{gubg_arduino}/cook/avr.chai")
        .option(arch)
        .output(output_dir)
        .generate(:ninja, uri)
        .ninja()
    Arduino.program("#{output_dir}/#{uri.gsub("/",".")}", arch: arch)
end

desc "Bus"
task :bus do
    cooker().generate(:ninja, "domotica/bus").ninja().run()
end

namespace :laurot do
    desc "Laurot slave"
    task :slave, [:id, :arch] do |t,args|
        uri, id, arch = "laurot/slave", args[:id]||0, args[:arch]||"mega"
        output_dir = "laurot/build/#{arch}_#{id}"
        cooker()
            .recipes_fn(gubg_arduino, "recipes.chai")
            .toolchain("gcc", "#{gubg_arduino}/cook/avr.chai")
            .option(arch)
            .option("slave.id", id)
            .output(output_dir)
            .generate(:ninja, uri)
            .ninja()
        Arduino.program("#{output_dir}/#{uri.gsub("/",".")}", arch: arch)
    end
    desc "Laurot master"
    task :master do
        cooker().generate(:ninja, "laurot/master").ninja().run("-t", "/dev/ttyUSB0", "-b", 9600, "-s", 1)
    end
end

namespace :quiz do
    desc "Quiz slave"
    task :slave, [:id, :arch] do |t,args|
        uri, id, arch = "quiz/slave", args[:id]||0, args[:arch]||"uno"
        output_dir = "quiz/build/#{arch}_#{id}"
        cooker()
            .recipes_fn(gubg_arduino, "recipes.chai")
            .toolchain("gcc", "#{gubg_arduino}/cook/avr.chai")
            .option(arch)
            .option("slave.id", id)
            .output(output_dir)
            .generate(:ninja, uri)
            .ninja()
        Arduino.program("#{output_dir}/#{uri.gsub("/",".")}", arch: arch)
    end
    desc "Quiz master"
    task :master do
        cooker().generate(:ninja, "quiz/master").ninja().run("-t", "/dev/ttyACM0", "-b", 9600)
    end
end
