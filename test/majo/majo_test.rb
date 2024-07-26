require_relative '../test_helper'

class MajoTest < Minitest::Test
  def test_smoke
    r = run_majo
    assert_kind_of Majo::Result, r
  end
end
