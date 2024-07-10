# frozen_string_literal: true

require_relative 'majo.so'
require_relative "majo/version"
require_relative 'majo/allocation_info'

module Majo
  class Error < StandardError; end

  def self.start
    GC.start
    GC.start
    GC.start
    __start
  end

  def self.stop
    GC.start
    GC.start
    GC.start
    __stop
  end

  def self.run
    r = start
    yield
    r
  ensure
    stop
  end
end
