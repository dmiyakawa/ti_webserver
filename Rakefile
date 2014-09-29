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

task :default => 'book.pdf'

file 'book.pdf' => ['config.yml', 'ti_webserver.re'] do
  sh "rm -rf book-pdf book.pdf"
  sh "review-pdfmaker config.yml"
end

task :clean do
  sh "rm -rf book-pdf book.pdf"
end
