require_relative '../../test_helper'

class MajoFormatterCsvTest < Minitest::Test
  def test_smoke
    r = run_majo
    io = StringIO.new
    r.report(out: io, formatter: :csv)
    assert CSV.parse io.string
  end
end
