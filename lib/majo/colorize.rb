module Majo
  module Colorize
    def blue(str)
      "\e[34m#{str}\e[0m"
    end

    def cyan(str)
      "\e[36m#{str}\e[0m"
    end
  end
end
