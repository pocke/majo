# frozen_string_literal: true

require "bundler/gem_tasks"

require "rubocop/rake_task"

RuboCop::RakeTask.new

require 'rake/testtask'

Rake::TestTask.new do |test|
  test.libs << 'test'
  test.test_files = Dir['test/**/*_test.rb']
  test.verbose = true
end

require "rake/extensiontask"

Rake::ExtensionTask.new("majo")

CLEAN.add("{ext,lib}/**/*.{o,so,bundle}", "pkg")

task test: :compile
task default: %i[rubocop test]
