require 'minitest'
require 'minitest/autorun'

require 'majo'

require 'rbs'
require 'stringio'

def run_majo
  Majo.run { RBS::Environment.from_loader(RBS::EnvironmentLoader.new).resolve_type_names }
end
