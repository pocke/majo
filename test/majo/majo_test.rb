require_relative '../test_helper'

class MajoTest < Minitest::Test
  def test_smoke
    require 'rbs'
    r = Majo.run { RBS::Environment.from_loader(RBS::EnvironmentLoader.new).resolve_type_names }
    assert_kind_of Majo::Result, r
  end
end
