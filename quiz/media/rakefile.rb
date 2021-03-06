task :default do
    sh "rake -T"
end

desc "Split ok/ko on silence"
task :split_ok_ko do
  sh("sox ok_ko.wav out.wav silence 1 0.1 1% 1 0.1 1% : newfile : restart")
end

desc "Encode all ok/ko .wav files"
task :encode_ok_ko do
    18.times do |i|
        %w[ok ko].each do |name|
            ifn = "#{name}%02d.wav"%i
            ofn = "#{name}%02d.ogg"%i
            sh("ffmpeg -i #{ifn} -vn -c:a libvorbis -q:a 0 #{ofn}")
        end
    end
end

desc "Encode all .mp4 files"
task :encode_mp4 do
    Dir.chdir("raw") do
        FileList.new("*.mp4").each do |ifn|
            ofn = ifn.gsub(".mp4", ".ogg")
            sh("ffmpeg -i #{ifn} -vn -c:a libvorbis -q:a 8 #{ofn}")
        end
    end
end
