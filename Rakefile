# frozen_string_literal: true

require "bundler/gem_tasks"

require "rubocop/rake_task"

RuboCop::RakeTask.new

task default: %i[rubocop]

require "rake/extensiontask"

Rake::ExtensionTask.new("majo")

CLEAN.add("{ext,lib}/**/*.{o,so,bundle}", "pkg")
