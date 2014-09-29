require 'fileutils'

def build(mode, chapter)
  sh "review-compile --target=#{mode} --footnotetext --stylesheet=style.css #{chapter} > tmp"
  mode_ext = {"html" => "html", "latex" => "tex",
              "idgxml" => "xml", "inao" => "inao"}
  FileUtils.mv "tmp", chapter.gsub(/re\z/, mode_ext[mode])
end

def build_all(mode)
  sh "review-compile --all --target=#{mode} --footnotetext --stylesheet=style.css"
end

def clean()
  sh "rm -rf webserver-pdf webserver.pdf"
  sh "rm -f webserver.txt webserver.html"
end

def build_pdf()
  clean()
  sh "review-pdfmaker config.yml"
end

desc "build html (Usage: rake build re=target.re)"
task :html do
  if ENV['re'].nil?
    puts "Usage: rake build re=target.re"
    exit
  end
  build("html", ENV['re'])
end

desc "build all html"
task :html_all do
  build_all("html")
end

desc "build all text"
task :text_all do
  build_all("text")
end

file 'webserver.pdf' => ['config.yml', 'webserver.re'] do
  build_pdf()
end


task :default => 'webserver.pdf'
task :clean do
  clean()
end
