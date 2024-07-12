require 'csv'

module Majo
  module Formatter
    class CSV
      def initialize(result)
        @result = result
      end

      def call
        ::CSV.generate do |csv|
          csv << ['Object class path', 'Class path', 'Method ID', 'Path', 'Line', 'Alloc generation', 'Free generation', 'Memsize']
          @result.allocations.each do |row|
            csv << [row.object_class_path, row.class_path, row.method_id, row.path, row.line, row.alloc_generation, row.free_generation, row.memsize]
          end
        end
      end
    end
  end
end
