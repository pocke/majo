# frozen_string_literal: true

require_relative 'majo.so'
require_relative "majo/version"
require_relative "majo/colorize"
require_relative 'majo/allocation_info'
require_relative 'majo/result'
require_relative 'majo/formatter'

module Majo
  class Error < StandardError; end

  def self.start(upper_lifetime: nil, lower_lifetime: 1)
    GC.start
    GC.start
    GC.start
    __start(upper_lifetime, lower_lifetime)
  end

  def self.stop
    GC.start
    GC.start
    GC.start
    res = __stop

    # Collect retained objcects
    ObjectSpace.each_object do |obj|
      res.store_retained_object(obj)
    end
  end

  def self.run(upper_lifetime: nil, lower_lifetime: 1)
    r = start(upper_lifetime:, lower_lifetime:)
    yield
    r
  ensure
    stop
  end
end
