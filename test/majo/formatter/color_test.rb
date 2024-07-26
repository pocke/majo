require_relative '../../test_helper'

class MajoFormatterColorTest < Minitest::Test
  def test_smoke
    r = run_majo
    io = StringIO.new
    r.report(out: io, formatter: :color)
  end
end
