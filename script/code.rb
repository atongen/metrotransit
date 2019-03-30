#!/usr/bin/env ruby

dir = File.expand_path("../..", __FILE__)
Dir.chdir(dir)

def select_parts(parts, strs)
  regexs = strs.map { |s| %r(#{s}) }
  parts.select do |p|
    regexs.any? { |r| r.match(p) } &&
      File.exists?(p)
  end
end

parts = ENV["PATH"].split(":")

ocaml = select_parts(parts, %w( opam ocaml ))
js = select_parts(parts, %w( nvm node ))
rest = parts - (ocaml + js)

path = (js + ocaml + rest).uniq.join(":")
system({ "PATH" => path }, `which code`.strip)
